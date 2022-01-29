#ifndef venus_rpc_vn2_rpc_h
#define venus_rpc_vn2_rpc_h

#ifdef __cplusplus
extern "C" {
#endif

int VN_Run_RPC_Server(const char * server_host, const int server_port, const char * server_name, const char * tracker_host, const int tracker_port);

#ifdef __cplusplus
}
#endif

#endif /* venus_rpc_vn2_rpc_h */
