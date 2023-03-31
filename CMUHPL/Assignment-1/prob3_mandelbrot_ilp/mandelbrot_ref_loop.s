        # This is the inner loop of mandel_ref
        # Parameters are passed to the function as follows:
        #   %xmm0: c_re
        #   %xmm1: c_im
        #   %edi:  count
        # Before entering the loop, the function sets registers
	# to initialize local variables:
        #   %xmm2: z_re = c_re
        #   %xmm3: z_im = c_im
        #   %eax:  i = 0
.L123:
        vmulss  %xmm2, %xmm2, %xmm4 ;将z_re*z_re的值存在xmm4中
        vmulss  %xmm3, %xmm3, %xmm5 ;将z_im*z_im的值存在xmm5中
        vaddss  %xmm5, %xmm4, %xmm6 ;将z_re*z_re+z_im*z_im的值存在xmm6中
        vucomiss        .LC0(%rip), %xmm6 ;将xmm6的值和4相比
        ja      .L126   ；大于则结束循环
        vaddss  %xmm2, %xmm2, %xmm2 ;将z_re*2
        addl    $1, %eax ;i+1
        cmpl    %edi, %eax   ;将eax和edi进行比较
        vmulss  %xmm3, %xmm2, %xmm3 ;z_re=z_re*z_re - z_im*z_im
        vsubss  %xmm5, %xmm4, %xmm2 ;z_re-=z_im
        vaddss  %xmm3, %xmm1, %xmm3 ;z_im=2.f * z_re * z_im;
        vaddss  %xmm2, %xmm0, %xmm2 ;z_im+=c_im
        jne     .L123 ;cmp比较结果，等于count束循环,不等于继续循环
