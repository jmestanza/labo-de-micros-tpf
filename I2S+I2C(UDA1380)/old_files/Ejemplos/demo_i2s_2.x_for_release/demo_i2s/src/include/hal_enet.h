

#define ENET_OK      0
#define ENET_ERR_SIZE 1

void hal_enet_init(void);
void hal_enet_init_set_mac_addr(const byte *addr);
bool hal_enet_r_ok(void);
byte* hal_enet_r_get_buf(void);
void hal_enet_r_clear_int(void);
void hal_enet_r_enable(void);
bool hal_enet_t_ok(void);
bool hal_enet_t_empty(void);
void hal_enet_t_clear_int(void);
int  hal_enet_t_send(byte *buf, uint len);
void hal_enet_loop_disable(void);
void hal_enet_r_clear_ok(void);
void hal_enet_r_set_ok(void);


