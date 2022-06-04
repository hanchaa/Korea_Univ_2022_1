.globl  csd_MMU_table_fig1
.globl  csd_MMU_table_fig2
.section .csd_mmu_tbl,"a"

csd_MMU_table_fig1:
.set SECT, 0
.word	SECT + 0x15de6		/* S=b1 TEX=b101 AP=b11, Domain=b1111, C=b0, B=b1 */
.set	SECT, SECT+0x100000
.word	SECT + 0x15de6		/* S=b1 TEX=b101 AP=b11, Domain=b1111, C=b0, B=b1 */
.word	csd_page_table_fig1 + 0x1e1		/* Domain=b1111 */

.align 14
csd_MMU_table_fig2:
.set SECT, 0
.word	SECT + 0x15de6		/* S=b1 TEX=b101 AP=b11, Domain=b1111, C=b0, B=b1 */
.set	SECT, SECT+0x100000
.word	SECT + 0x15de6		/* S=b1 TEX=b101 AP=b11, Domain=b1111, C=b0, B=b1 */
.word	csd_page_table_fig2 + 0x1e1		/* Domain=b1111 */


.section .csd_mmu_tbl_lv2, "a"
csd_page_table_fig1:
.set PAGE, 0x400000    // 0x400000
.word PAGE + 0x576 // S=b1 TEX=b101 AP=b11 C=b0 B=b1
.set PAGE, PAGE+0x1000 // 0x401000
.word PAGE + 0x576 // S=b1 TEX=b101 AP=b11 C=b0 B=b1
.set PAGE, PAGE+0x1000 // 0x402000
.word PAGE + 0x576 // S=b1 TEX=b101 AP=b11 C=b0 B=b1

.align 10
csd_page_table_fig2:
.set PAGE, 0x400000    //0x400000
.word PAGE + 0x576 // S=b1 TEX=b101 AP=b11 C=b0 B=b1
.set PAGE, PAGE+0x2000 //0x402000
.word PAGE + 0x576 // S=b1 TEX=b101 AP=b11 C=b0 B=b1
.set PAGE, PAGE-0x2000 //0x400000
.word PAGE + 0x576 // S=b1 TEX=b101 AP=b11 C=b0 B=b1
.end
