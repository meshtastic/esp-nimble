/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_BLE_HS_TEST_UTIL_
#define H_BLE_HS_TEST_UTIL_

#include <inttypes.h>
#include "host/ble_gap.h"
#include "ble_hs_priv.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_conn;
struct ble_l2cap_chan;
struct hci_disconn_complete;
struct hci_create_conn;

extern struct os_eventq ble_hs_test_util_evq;
extern const struct ble_gap_adv_params ble_hs_test_util_adv_params;

struct ble_hs_test_util_num_completed_pkts_entry {
    uint16_t handle_id; /* 0 for terminating entry in array. */
    uint16_t num_pkts;
};

struct ble_hs_test_util_flat_attr {
    uint16_t handle;
    uint16_t offset;
    uint8_t value[BLE_ATT_ATTR_MAX_LEN];
    uint16_t value_len;
};

struct ble_hs_test_util_mbuf_params {
    unsigned prev_tx:1;
    unsigned rx_queue:1;
    unsigned prep_list:1;
};

struct ble_hs_test_util_att_info_entry {
    uint16_t handle;        /* 0 on last entry */
    const ble_uuid_t *uuid;
};

struct ble_hs_test_util_att_group_type_entry {
    uint16_t start_handle;  /* 0 on last entry */
    uint16_t end_handle;    /* 0 on last entry */
    const ble_uuid_t *uuid;
};

#define BLE_HS_TEST_UTIL_L2CAP_HCI_HDR(handle, pb, len) \
    ((struct hci_data_hdr) {                            \
        .hdh_handle_pb_bc = ((handle)  << 0) |          \
                            ((pb)      << 12),          \
        .hdh_len = (len)                                \
    })

void ble_hs_test_util_prev_tx_enqueue(struct os_mbuf *om);
struct os_mbuf *ble_hs_test_util_prev_tx_dequeue(void);
struct os_mbuf *ble_hs_test_util_prev_tx_dequeue_pullup(void);
int ble_hs_test_util_prev_tx_queue_sz(void);
void ble_hs_test_util_prev_tx_queue_clear(void);

void ble_hs_test_util_set_ack_params(uint16_t opcode, uint8_t status,
                                     void *params, uint8_t params_len);
void ble_hs_test_util_set_ack(uint16_t opcode, uint8_t status);
void *ble_hs_test_util_get_first_hci_tx(void);
void *ble_hs_test_util_get_last_hci_tx(void);
void ble_hs_test_util_enqueue_hci_tx(void *cmd);
void ble_hs_test_util_prev_hci_tx_clear(void);
void ble_hs_test_util_build_cmd_complete(uint8_t *dst, int len,
                                         uint8_t param_len, uint8_t num_pkts,
                                         uint16_t opcode);
void ble_hs_test_util_build_cmd_status(uint8_t *dst, int len,
                                       uint8_t status, uint8_t num_pkts,
                                       uint16_t opcode);
void ble_hs_test_util_create_rpa_conn(uint16_t handle, uint8_t own_addr_type,
                                      const uint8_t *our_rpa,
                                      uint8_t peer_addr_type,
                                      const uint8_t *peer_id_addr,
                                      const uint8_t *peer_rpa,
                                      ble_gap_event_fn *cb, void *cb_arg);
void ble_hs_test_util_create_conn(uint16_t handle, const uint8_t *addr,
                                  ble_gap_event_fn *cb, void *cb_arg);
int ble_hs_test_util_connect(uint8_t own_addr_type,
                                   uint8_t peer_addr_type,
                                   const uint8_t *peer_addr,
                                   int32_t duration_ms,
                                   const struct ble_gap_conn_params *params,
                                   ble_gap_event_fn *cb,
                                   void *cb_arg,
                                   uint8_t ack_status);
int ble_hs_test_util_conn_cancel(uint8_t ack_status);
void ble_hs_test_util_conn_cancel_full(void);
void ble_hs_test_util_set_ack_disconnect(uint8_t hci_status);
int ble_hs_test_util_conn_terminate(uint16_t conn_handle, uint8_t hci_status);
void ble_hs_test_util_rx_disconn_complete(uint16_t conn_handle,
                                          uint8_t reason);
void ble_hs_test_util_conn_disconnect(uint16_t conn_handle);
int ble_hs_test_util_exp_hci_status(int cmd_idx, int fail_idx,
                                    uint8_t fail_status);
int ble_hs_test_util_disc(uint8_t own_addr_type, int32_t duration_ms,
                          const struct ble_gap_disc_params *disc_params,
                          ble_gap_event_fn *cb, void *cb_arg, int fail_idx,
                          uint8_t fail_status);
int ble_hs_test_util_disc_cancel(uint8_t ack_status);
void ble_hs_test_util_verify_tx_disconnect(uint16_t handle, uint8_t reason);
void ble_hs_test_util_verify_tx_create_conn(const struct hci_create_conn *exp);
int ble_hs_test_util_adv_set_fields(const struct ble_hs_adv_fields *adv_fields,
                                    int cmd_fail_idx, uint8_t hci_status);
int ble_hs_test_util_adv_rsp_set_fields(
    const struct ble_hs_adv_fields *adv_fields,
    int cmd_fail_idx, uint8_t hci_status);
int ble_hs_test_util_adv_start(uint8_t own_addr_type,
                               uint8_t peer_addr_type,
                               const uint8_t *peer_addr,
                               const struct ble_gap_adv_params *adv_params,
                               int32_t duration_ms,
                               ble_gap_event_fn *cb, void *cb_arg,
                               int fail_idx, uint8_t fail_status);
int ble_hs_test_util_adv_stop(uint8_t hci_status);
int ble_hs_test_util_wl_set(struct ble_gap_white_entry *white_list,
                            uint8_t white_list_count,
                            int fail_idx, uint8_t fail_status);
int ble_hs_test_util_conn_update(uint16_t conn_handle,
                                 struct ble_gap_upd_params *params,
                                 uint8_t hci_status);
int ble_hs_test_util_set_our_irk(const uint8_t *irk, int fail_idx,
                                 uint8_t hci_status);
int ble_hs_test_util_security_initiate(uint16_t conn_handle,
                                       uint8_t hci_status);
int ble_hs_test_util_l2cap_rx_first_frag(uint16_t conn_handle, uint16_t cid,
                                         struct hci_data_hdr *hci_hdr,
                                         struct os_mbuf *om);
int ble_hs_test_util_l2cap_rx(uint16_t conn_handle,
                              struct hci_data_hdr *hci_hdr,
                              struct os_mbuf *om);
int ble_hs_test_util_l2cap_rx_payload_flat(uint16_t conn_handle, uint16_t cid,
                                           const void *data, int len);
void ble_hs_test_util_rx_hci_buf_size_ack(uint16_t buf_size);
int ble_hs_test_util_rx_att_mtu_cmd(uint16_t conn_handle, int is_req,
                                    uint16_t mtu);
int ble_hs_test_util_rx_att_find_info_req(uint16_t conn_handle,
                                          uint16_t start_handle,
                                          uint16_t end_handle);
int ble_hs_test_util_rx_att_find_type_value_req(uint16_t conn_handle,
                                                uint16_t start_handle,
                                                uint16_t end_handle,
                                                uint16_t attr_type,
                                                const void *attr_val,
                                                uint16_t attr_len);
int ble_hs_test_util_rx_att_read_type_req(uint16_t conn_handle,
                                          uint16_t start_handle,
                                          uint16_t end_handle,
                                          const ble_uuid_t *uuid);
int ble_hs_test_util_rx_att_read_type_req16(uint16_t conn_handle,
                                            uint16_t start_handle,
                                            uint16_t end_handle,
                                            uint16_t uuid16);
int ble_hs_test_util_rx_att_read_req(uint16_t conn_handle,
                                     uint16_t attr_handle);
int ble_hs_test_util_rx_att_read_blob_req(uint16_t conn_handle,
                                          uint16_t attr_handle,
                                          uint16_t offset);
int ble_hs_test_util_rx_att_read_mult_req(uint16_t conn_handle,
                                          const uint16_t *handles,
                                          int num_handles);
int ble_hs_test_util_rx_att_read_group_type_req(uint16_t conn_handle,
                                                uint16_t start_handle,
                                                uint16_t end_handle,
                                                const ble_uuid_t *uuid);
int ble_hs_test_util_rx_att_read_group_type_req16(uint16_t conn_handle,
                                                  uint16_t start_handle,
                                                  uint16_t end_handle,
                                                  uint16_t uuid16);
int ble_hs_test_util_rx_att_write_req(uint16_t conn_handle,
                                      uint16_t attr_handle,
                                      const void *attr_val,
                                      uint16_t attr_len);
int ble_hs_test_util_rx_att_write_cmd(uint16_t conn_handle,
                                      uint16_t attr_handle,
                                      const void *attr_val,
                                      uint16_t attr_len);
int ble_hs_test_util_rx_att_prep_write_req(uint16_t conn_handle,
                                           uint16_t attr_handle,
                                           uint16_t offset,
                                           const void *attr_val,
                                           uint16_t attr_len);
int ble_hs_test_util_rx_att_exec_write_req(uint16_t conn_handle,
                                           uint8_t flags);
int ble_hs_test_util_rx_att_notify_req(uint16_t conn_handle,
                                       uint16_t attr_handle,
                                       void *attr_val,
                                       uint16_t attr_len);
int ble_hs_test_util_rx_att_indicate_req(uint16_t conn_handle,
                                         uint16_t attr_handle,
                                         void *attr_val,
                                         uint16_t attr_len);
void ble_hs_test_util_rx_att_err_rsp(uint16_t conn_handle, uint8_t req_op,
                                     uint8_t error_code, uint16_t err_handle);
void ble_hs_test_util_set_startup_acks(void);
void ble_hs_test_util_rx_num_completed_pkts_event(
    struct ble_hs_test_util_num_completed_pkts_entry *entries);
void ble_hs_test_util_rx_disconn_complete_event(
    struct hci_disconn_complete *evt);
uint8_t *ble_hs_test_util_verify_tx_hci(uint8_t ogf, uint16_t ocf,
                                        uint8_t *out_param_len);
void ble_hs_test_util_tx_all(void);
void ble_hs_test_util_verify_tx_prep_write(uint16_t attr_handle,
                                           uint16_t offset,
                                           const void *data, int data_len);
void ble_hs_test_util_verify_tx_exec_write(uint8_t expected_flags);
void ble_hs_test_util_verify_tx_read_rsp(uint8_t *attr_data, int attr_len);
void ble_hs_test_util_verify_tx_read_blob_rsp(uint8_t *attr_data,
                                              int attr_len);
void ble_hs_test_util_verify_tx_write_rsp(void);
void ble_hs_test_util_verify_tx_find_info_rsp(
    struct ble_hs_test_util_att_info_entry *entries);
void ble_hs_test_util_verify_tx_mtu_cmd(int is_req, uint16_t mtu);
void ble_hs_test_util_verify_tx_read_group_type_rsp(
    struct ble_hs_test_util_att_group_type_entry *entries);
void ble_hs_test_util_verify_tx_err_rsp(uint8_t req_op, uint16_t handle,
                                        uint8_t error_code);
uint8_t ble_hs_test_util_verify_tx_l2cap_update_req(
    struct ble_l2cap_sig_update_params *params);
int ble_hs_test_util_rx_l2cap_update_rsp(uint16_t conn_handle,
                                         uint8_t id, uint16_t result);
void ble_hs_test_util_verify_tx_l2cap_update_rsp(uint8_t exp_id,
                                                 uint16_t exp_result);
void ble_hs_test_util_set_static_rnd_addr(void);
struct os_mbuf *ble_hs_test_util_om_from_flat(const void *buf, uint16_t len);
int ble_hs_test_util_flat_attr_cmp(const struct ble_hs_test_util_flat_attr *a,
                                   const struct ble_hs_test_util_flat_attr *b);
void ble_hs_test_util_attr_to_flat(struct ble_hs_test_util_flat_attr *flat,
                                   const struct ble_gatt_attr *attr);
void ble_hs_test_util_attr_from_flat(
    struct ble_gatt_attr *attr, const struct ble_hs_test_util_flat_attr *flat);
int ble_hs_test_util_read_local_flat(uint16_t attr_handle, uint16_t max_len,
                                     void *buf, uint16_t *out_len);
int ble_hs_test_util_write_local_flat(uint16_t attr_handle,
                                      const void *buf, uint16_t buf_len);
int ble_hs_test_util_gatt_write_flat(uint16_t conn_handle,
                                     uint16_t attr_handle,
                                     const void *data, uint16_t data_len,
                                     ble_gatt_attr_fn *cb, void *cb_arg);
int ble_hs_test_util_gatt_write_no_rsp_flat(uint16_t conn_handle,
                                            uint16_t attr_handle,
                                            const void *data,
                                            uint16_t data_len);
int ble_hs_test_util_gatt_write_long_flat(uint16_t conn_handle,
                                          uint16_t attr_handle,
                                          const void *data, uint16_t data_len,
                                          ble_gatt_attr_fn *cb, void *cb_arg);
struct os_mbuf *ble_hs_test_util_mbuf_alloc_all_but(int count);
int ble_hs_test_util_mbuf_count(
    const struct ble_hs_test_util_mbuf_params *params);
void ble_hs_test_util_assert_mbufs_freed(
    const struct ble_hs_test_util_mbuf_params *params);
void ble_hs_test_util_post_test(void *arg);
int ble_hs_test_util_num_cccds(void);
int ble_hs_test_util_num_our_secs(void);
int ble_hs_test_util_num_peer_secs(void);
void ble_hs_test_util_init_no_start(void);
void ble_hs_test_util_init(void);

#ifdef __cplusplus
}
#endif

#endif
