/*******************************************************************************
* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef CPU_X64_LRN_JIT_AVX512_COMMON_LRN_FWD_BLOCKED_HPP
#define CPU_X64_LRN_JIT_AVX512_COMMON_LRN_FWD_BLOCKED_HPP

#include "cpu/x64/lrn/jit_avx512_common_lrn_fwd_base.hpp"
#include "cpu/x64/lrn/jit_avx512_common_lrn_utils.hpp"

namespace dnnl {
namespace impl {
namespace cpu {
namespace x64 {
namespace lrn {

template <data_type_t d_type>
class jit_avx512_common_lrn_kernel_fwd_blocked_t
    : public jit_avx512_common_lrn_kernel_fwd_t<d_type> {

public:
    DECLARE_CPU_JIT_AUX_FUNCTIONS(jit_avx512_common_lrn_kernel_fwd_t::
                    jit_avx512_common_lrn_kernel_fwd_blocked_t)

    jit_avx512_common_lrn_kernel_fwd_blocked_t(const struct nChw16c_across_t &J,
            prop_kind_t prop_kind, int use_h_parallel, float A, float K,
            void *code_ptr = nullptr,
            size_t code_size = 2 * Xbyak::DEFAULT_MAX_CODE_SIZE);
    ~jit_avx512_common_lrn_kernel_fwd_blocked_t() = default;

    void compute_loop(int loop_size_param);
    void (*ker)(
            typename jit_avx512_common_lrn_kernel_fwd_t<d_type>::jit_args_fwd_t
                    *);
    void operator()(
            typename jit_avx512_common_lrn_kernel_fwd_t<d_type>::jit_args_fwd_t
                    *arg) {
        ker(arg);
    }

private:
    using data_t = typename jit_avx512_common_lrn_kernel_fwd_t<d_type>::data_t;

    int xmm_size_, zmm_size_, buffer_block_, buffer_nest_offset_,
            src_prev_offset_, HW_, W_;
    across_version version_;
    Reg64 t_ = rsp;
    Reg64 hw_ = r9;

    static constexpr int xsrc_prev_ = 2;
    static constexpr int xsrc_next_ = 3;
    int use_h_parallelism_;
};

} // namespace lrn
} // namespace x64
} // namespace cpu
} // namespace impl
} // namespace dnnl

#endif
