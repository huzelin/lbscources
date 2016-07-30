#include "server/server.h"
#include "server/query.h"
#include "boost/lexical_cast.hpp"
#include "common/log.h"
#include "common/posix-time.h"
#include "proto/mod-update.pb.h"
#include "jansson.h"
#include <string.h>

namespace mod {

bool Server::Start() {
  lbs_grid_index_init();
  if (!StartHttpServer()) {
    LOG_ERROR("start http server failed");
    return false;
  }
  return true;
}

// start http server
bool Server::StartHttpServer() {
  evbase_t* evbase = event_base_new();
  if (evbase == NULL) {
    LOG_ERROR("event base new failed");
    return false;
  }
  evhtp_t* evhtp   = evhtp_new(evbase, NULL);
  if (evhtp == NULL) {
    LOG_ERROR("evhtp new failed");
    return false;
  }

  evhtp_set_gencb(evhtp, (evhtp_callback_cb)ProcessQuery, this);
  evhtp_set_cb(evhtp, "/mod_update", (evhtp_callback_cb)ProcessUpdate, this);
  evhtp_use_threads(evhtp, NULL, threads_num_, this);
  evhtp_bind_socket(evhtp, ip_.c_str(), port_, 10240);
  event_base_loop(evbase,  0);
  event_base_free(evbase);
  return true;
}

void Server::ProcessUpdate(evhtp_request_t* request, void* arg) {
  Server* server = reinterpret_cast<Server*>(arg);
  server->ProcessUpdate(request);
}

void Server::ProcessQuery(evhtp_request_t* request, void* arg) {
  Server* server = reinterpret_cast<Server*>(arg);
  server->ProcessQuery(request);
}

// 处理更新请求
void Server::ProcessUpdate(evhtp_request_t* request) {
  evhtp_request_pause(request);
  evhtp_send_reply(request, EVHTP_RES_OK);
  int data_length = evbuffer_get_length(request->buffer_in);
  if (data_length <= 0) {
    LOG_ERROR("accept udapet request error");
  } else {
    const char* data = reinterpret_cast<const char*>(evbuffer_pullup(request->buffer_in, -1));
    ModUpdateRequest mod_update_request;
    if (!mod_update_request.ParseFromArray(data, data_length)) {
      LOG_ERROR("parse ModUpdateRequest error");
    } else {
      double lon = mod_update_request.lon();
      double lat = mod_update_request.lat();
      uint64_t timestamp = mod_update_request.timestamp();
      uint32_t id = boost::lexical_cast<uint32_t>(mod_update_request.id());
      lbs_grid_index_update(lon, lat, timestamp, id);
      LOG_DEBUG("accpet mod update, id=%d, lon=%f, lat=%f, timestamp=%ld",
                id, lon, lat, timestamp);
    }
  }
  evhtp_request_resume(request);
}

// 处理查询请求
void Server::ProcessQuery(evhtp_request_t* request) {
  evhtp_request_pause(request);
  struct evkeyvalq http_query;
  evhttp_parse_query_str((char*)request->uri->query_raw, &http_query); 
  LOG_DEBUG("get request: %s", request->uri->query_raw);
  Query query;
  if (query.Parse(http_query) == false) {
    evbuffer_add_printf(request->buffer_out, "Invalid Request\n");
    evhtp_send_reply(request, EVHTP_RES_OK);
    evhtp_request_resume(request);
  }
  lbs_res_node_t res_node;
  lbs_queue_init(&(res_node.queue));
  if (strcmp(query.query_type().c_str(), kRangeQuery.c_str()) == 0) {
    // Range查询
    double lon1, lon2, lat1,lat2;
    query.GetRange(lon1, lon2, lat1, lat2);
    uint64_t time_start = common::GetTime(); 
    lbs_grid_index_range_query(lon1, lon2, lat1, lat2, &res_node);
    uint64_t time_end = common::GetTime();
    AssembleResult(request, &res_node, time_end - time_start);
  } else if (strcmp(query.query_type().c_str(), kNNQuery.c_str()) == 0) {
    // NN查询
    double lon, lat;
    query.GetPos(lon, lat);
    uint64_t time_start = common::GetTime();
    lbs_grid_index_nn_query(lon, lat, &res_node);
    uint64_t time_end = common::GetTime();
    AssembleResult(request, &res_node, time_end - time_start);
  } else {
    LOG_ERROR("unkown query");
  }
  evhtp_send_reply(request, EVHTP_RES_OK);
  evhtp_request_resume(request);
}

// range query
void Server::AssembleResult(evhtp_request_t* request, lbs_res_node_t* res_node, uint64_t time) {
  json_t* data_array = json_array();
  lbs_queue_t* head  = &(res_node->queue);
  lbs_queue_t* p     = head->next;
  for (; p != head;) {
    lbs_res_node_t* rn = (lbs_res_node_t*)(p);
    json_t* new_doc = json_object();
    // set id
    json_object_set_new(new_doc, "id", json_integer(rn->id));
    // set lon
    json_object_set_new(new_doc, "lon", json_real(rn->lon));
    // set lat
    json_object_set_new(new_doc, "lat", json_real(rn->lat)); 
    // set stamp
    json_object_set_new(new_doc, "timestamp", json_integer(rn->timestamp));
    json_array_append(data_array, new_doc);
    json_decref(new_doc);
    // 跳到下一个节点
    p = p->next;
    free(rn);
  }
  json_t* root = json_object();
  json_object_set(root, "list", data_array);
  json_decref(data_array);

  json_object_set_new(root, "rt", json_integer(time));
  char* out = json_dumps(root, JSON_ESCAPE_SLASH);
  json_decref(root);

  evbuffer_add_printf(request->buffer_out, "%s", out);
  free(out);
}

}  // namespace mod
