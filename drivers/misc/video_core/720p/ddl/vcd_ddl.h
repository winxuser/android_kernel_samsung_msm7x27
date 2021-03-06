/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _VCD_DDL_H_
#define _VCD_DDL_H_
#include "vcd_ddl_api.h"
#include "vcd_ddl_utils.h"
#include "vcd_ddl_firmware.h"
#include "video_core_720p.h"

#define DDL_BUSY_STATE 1
#define DDL_IDLE_STATE 0
#define DDL_ERROR_STATE 2
#define DDL_IS_BUSY(ddl_context) \
	(((ddl_context)->ddl_busy != DDL_IDLE_STATE))
#define DDL_BUSY(ddl_context) \
	((ddl_context)->ddl_busy = DDL_BUSY_STATE)
#define DDL_IDLE(ddl_context) \
	((ddl_context)->ddl_busy = DDL_IDLE_STATE)
#define DDL_ERROR(ddl_context) \
	((ddl_context)->ddl_busy = DDL_ERROR_STATE)

#define DDL_DEVICE_NOTINIT  0
#define DDL_DEVICE_INITED   1
#define DDL_DEVICE_HWFATAL  2
#define DDL_IS_INITIALIZED(ddl_context)  \
	(ddl_context->device_state == DDL_DEVICE_INITED)

#define DDLCOMMAND_STATE_IS(ddl_context, command_state) \
	(command_state == (ddl_context)->cmd_state)

#define DDLCLIENT_STATE_IS(ddl, cs) \
	(cs == (ddl)->client_state)

#define DDL_DPB_OP_INIT       1
#define DDL_DPB_OP_MARK_FREE  2
#define DDL_DPB_OP_MARK_BUSY  3
#define DDL_DPB_OP_SET_MASK   4
#define DDL_DPB_OP_RETRIEVE   5

#define DDL_INIT_CLIENTS     0
#define DDL_GET_CLIENT       1
#define DDL_FREE_CLIENT      2
#define DDL_ACTIVE_CLIENT    3

#define DDL_INVALID_CHANNEL_ID  ((u32)~0)
#define DDL_INVALID_CODEC_TYPE ((u32)~0)
#define DDL_INVALID_INTR_STATUS ((u32)~0)

#define DDL_ENC_REQ_IFRAME                      0x1
#define DDL_ENC_CHANGE_IPERIOD                  0x2
#define DDL_ENC_CHANGE_BITRATE                  0x4
#define DDL_ENC_CHANGE_FRAMERATE                0x8

#define DDL_DEC_REQ_OUTPUT_FLUSH                0x1

struct ddl_dma_buffer {
	void *virt_addr;
	phys_addr_t phys_addr;
	size_t size;
};

enum ddl_cmd_state {
	DDL_CMD_INVALID = 0x0,
	DDL_CMD_DMA_INIT = 0x1,
	DDL_CMD_CPU_RESET = 0x2,
	DDL_CMD_CHANNEL_SET = 0x3,
	DDL_CMD_INIT_CODEC = 0x4,
	DDL_CMD_HEADER_PARSE = 0x5,
	DDL_CMD_DECODE_SET_DPB = 0x6,
	DDL_CMD_DECODE_FRAME = 0x7,
	DDL_CMD_ENCODE_FRAME = 0x8,
	DDL_CMD_EOS = 0x9,
	DDL_CMD_CHANNEL_END = 0xA,
	DDL_CMD_32BIT = 0x7FFFFFFF
};

enum ddl_client_state {
	DDL_CLIENT_INVALID = 0x0,
	DDL_CLIENT_OPEN = 0x1,
	DDL_CLIENT_WAIT_FOR_CHDONE = 0x2,
	DDL_CLIENT_WAIT_FOR_INITCODEC = 0x3,
	DDL_CLIENT_WAIT_FOR_INITCODECDONE = 0x4,
	DDL_CLIENT_WAIT_FOR_DPB = 0x5,
	DDL_CLIENT_WAIT_FOR_DPBDONE = 0x6,
	DDL_CLIENT_WAIT_FOR_FRAME = 0x7,
	DDL_CLIENT_WAIT_FOR_FRAME_DONE = 0x8,
	DDL_CLIENT_WAIT_FOR_EOS_DONE = 0x9,
	DDL_CLIENT_WAIT_FOR_CHEND = 0xA,
	DDL_CLIENT_FATAL_ERROR = 0xB,
	DDL_CLIENT_32BIT = 0x7FFFFFFF
};

struct ddl_mask {
	u32 client_mask;
	u32 hw_mask;
};

struct ddl_context;

struct ddl_client_context;

struct ddl_codec_data_hdr {
	u32 decoding;
};

struct ddl_encoder_data {
	struct ddl_codec_data_hdr hdr;
	struct vcd_property_codec codec_type;
	struct vcd_property_frame_size frame_size;
	struct vcd_property_frame_rate frame_rate;
	struct vcd_property_target_bitrate target_bit_rate;
	struct vcd_property_profile profile;
	struct vcd_property_level level;
	struct vcd_property_rate_control rc_type;
	struct vcd_property_multi_slice multi_slice;
	u32 meta_data_enable_flag;
	u32 suffix;
	struct ddl_dma_buffer meta_data_input;
	phys_addr_t meta_data_offset;
	struct vcd_property_short_header short_header;
	struct vcd_property_vop_timing vop_timing;
	u32 hdr_ext_control;
	struct vcd_property_db_config db_control;
	struct vcd_property_entropy_control entropy_control;
	struct vcd_property_i_period period;
	struct vcd_property_session_qp session_qp;
	struct vcd_property_qp_range qp_range;
	struct vcd_property_rc_level rc_level;
	u32 r_cframe_skip;
	u32 vb_vbuffer_size;
	struct vcd_property_frame_level_rc_params frame_level_rc;
	struct vcd_property_adaptive_rc_params adaptive_rc;
	struct vcd_property_intra_refresh_mb_number intra_refresh;
	struct vcd_property_buffer_format buf_format;
	struct vcd_property_buffer_format re_con_buf_format;
	u32 dynamic_prop_change;
	u32 dynmic_prop_change_req;
	u32 ext_enc_control_val;
	struct vidc_720p_enc_frame_info enc_frame_info;
	struct ddl_dma_buffer enc_dpb_addr;
	struct ddl_dma_buffer seq_header;
	struct vcd_buffer_requirement input_buf_req;
	struct vcd_buffer_requirement output_buf_req;
	struct vcd_buffer_requirement client_input_buf_req;
	struct vcd_buffer_requirement client_output_buf_req;
};

struct ddl_decoder_data {
	struct ddl_codec_data_hdr hdr;
	struct vcd_property_codec codec_type;
	struct vcd_property_buffer_format buf_format;
	struct vcd_property_frame_size frame_size;
	struct vcd_property_frame_size client_frame_size;
	struct vcd_property_profile profile;
	struct vcd_property_level level;
	u32 progressive_only;
	u32 meta_data_enable_flag;
	u32 suffix;
	struct ddl_dma_buffer meta_data_input;
	struct ddl_dma_buffer ref_buffer;
	size_t meta_data_offset;
	struct vcd_property_post_filter post_filter;
	struct vcd_phys_sequence_hdr decode_config;
	u32 header_in_start;
	u32 min_dpb_num;
	size_t y_cb_cr_size;
	struct ddl_property_dec_pic_buffers dp_buf;
	struct ddl_mask dpb_mask;
	u32 dynamic_prop_change;
	u32 dynmic_prop_change_req;
	struct vidc_720p_dec_disp_info dec_disp_info;
	struct ddl_dma_buffer dpb_comv_buffer;
	struct ddl_dma_buffer h264Vsp_temp_buffer;
	struct vcd_buffer_requirement actual_input_buf_req;
	struct vcd_buffer_requirement min_input_buf_req;
	struct vcd_buffer_requirement client_input_buf_req;
	struct vcd_buffer_requirement actual_output_buf_req;
	struct vcd_buffer_requirement min_output_buf_req;
	struct vcd_buffer_requirement client_output_buf_req;
};

union ddl_codec_data {
	struct ddl_codec_data_hdr hdr;
	struct ddl_decoder_data decoder;
	struct ddl_encoder_data encoder;
};

struct ddl_context {
	u8 *core_virtual_base_addr;
	void (*ddl_callback) (u32 event, u32 status, void *payload, u32 size,
			      u32 *ddl_handle, void *const client_data);
	void *client_data;
	void (*pf_interrupt_clr) (void);
	enum ddl_cmd_state cmd_state;
	struct ddl_client_context *current_ddl;
	struct ddl_dma_buffer context_buf_addr;
	struct ddl_dma_buffer db_line_buffer;
	struct ddl_dma_buffer data_partition_tempbuf;
	struct ddl_dma_buffer metadata_shared_input;
	struct ddl_dma_buffer dbg_core_dump;
	u32 enable_dbg_core_dump;
	struct ddl_client_context *ddl_clients[VCD_MAX_NO_CLIENT];
	u32 device_state;
	u32 ddl_busy;
	u32  intr_status;
	u32 cmd_err_status;
	u32 disp_pic_err_status;
	u32 op_failed;
};

struct ddl_client_context {
	struct ddl_context *ddl_context;
	enum ddl_client_state client_state;
	u32 decoding;
	u32 channel_id;
	struct ddl_frame_data_tag input_frame;
	struct ddl_frame_data_tag output_frame;
	union ddl_codec_data codec_data;
};

struct ddl_context *ddl_get_context(void);
void ddl_move_command_state(struct ddl_context *ddl_context,
	enum ddl_cmd_state command_state);
void ddl_move_client_state(struct ddl_client_context *ddl,
	enum ddl_client_state client_state);
void ddl_core_init(struct ddl_context *);
void ddl_core_start_cpu(struct ddl_context *);
void ddl_channel_set(struct ddl_client_context *);
void ddl_channel_end(struct ddl_client_context *);
void ddl_encode_init_codec(struct ddl_client_context *);
void ddl_decode_init_codec(struct ddl_client_context *);
void ddl_encode_frame_run(struct ddl_client_context *);
void ddl_decode_frame_run(struct ddl_client_context *);
void ddl_decode_eos_run(struct ddl_client_context *);
void ddl_release_context_buffers(struct ddl_context *);
void ddl_release_client_internal_buffers(struct ddl_client_context *ddl);
u32 ddl_decode_set_buffers(struct ddl_client_context *);
u32 ddl_decoder_dpb_transact(struct ddl_decoder_data *dec,
	struct ddl_frame_data_tag *in_out_frame, u32 operation);
u32 ddl_client_transact(u32, struct ddl_client_context **);
void ddl_set_default_decoder_buffer_req(struct ddl_decoder_data *dec,
	u32 estimate);
void ddl_set_default_encoder_buffer_req(struct ddl_encoder_data *enc);
void ddl_set_default_dec_property(struct ddl_client_context *);
u32 ddl_encoder_ready_to_start(struct ddl_client_context *);
u32 ddl_decoder_ready_to_start(struct ddl_client_context *,
	struct vcd_phys_sequence_hdr *);
size_t ddl_get_yuv_buffer_size(struct vcd_property_frame_size *frame_size,
	struct vcd_property_buffer_format *buf_format, u32 interlace);
void ddl_calculate_stride(struct vcd_property_frame_size *frame_size,
	u32 interlace);
void ddl_encode_dynamic_property(struct ddl_client_context *ddl, u32 enable);
void ddl_decode_dynamic_property(struct ddl_client_context *ddl, u32 enable);
void ddl_set_initial_default_values(struct ddl_client_context *ddl);
u32 ddl_handle_core_errors(struct ddl_context *ddl_context);
void ddl_client_fatal_cb(struct ddl_context *ddl_context);
void ddl_hw_fatal_cb(struct ddl_context *ddl_context);
u32 ddl_hal_engine_reset(struct ddl_context *ddl_context);

#endif
