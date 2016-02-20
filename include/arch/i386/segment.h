/*
 * LICENSE NOTES:
 * Since most i386 code comes from MIT xv6, I wonder how we should put the
 * license information...
 */

#ifndef _ASM_SEGMENT_H
#define _ASM_SEGMENT_H

/* The segment indices our i386 operating system is using */
#define SEG_KCODE 1  // kernel code
#define SEG_KDATA 2  // kernel data+stack
#define SEG_KCPU  3  // kernel per-cpu data
#define SEG_UCODE 4  // user code
#define SEG_UDATA 5  // user data+stack
#define SEG_TSS   6  // this process's task state

#ifndef __ASSEMBLY__
/*
 * Segment descriptor structure
 */
struct segdesc {
	unsigned int	lim_15_0:16;	/* Limit: Bits 15-0 */
	unsigned int	base_15_0:16;	/* Base: Bits 15-0 */
	unsigned int	base_23_16:8;	/* Base: Bits 23-16 */
	unsigned int	type:4;	/* Segment type, see STA_ constants */
	unsigned int	s:1;	/* 0 if system segment, 1 if application */
	unsigned int	dpl:2;	/* Privilege, see DPL_ constants */
	unsigned int	p:1;	/* Present */
	unsigned int	lim_19_16:4;	/* Limit: Bits 19-16 */
	unsigned int	avl:1;	/* Reserved for software use */
	unsigned int	rsv1:1;	/* Reserved for future */
	unsigned int	db:1;	/* 1 if 32-bit */
	unsigned int	g:1;	/* Limit is page or byte granularity */
	unsigned int	base_31_24:8;	/* Base: Bits 31-24 */
}

/*
 * For present segments, we usually only care the following properties:
 * (1) segment type (readable/writable/executable/etc.)
 * (2) segment base address
 * (3) segment size (or, number of bytes/pages).
 * (4) segment privilege (user or kernel)
 *
 * Setting the page granularity bit (g) does not necessarily need the
 * paging mechanism (default off) to be turned on, so we use page
 * granularity by default.
 */
#define SEG(type, base, size, dpl)	(struct segdesc) {	\
	((size) >> 12) & 0xffff,			\
	(unsigned int)(base) & 0xffff,		\
	((unsigned int)(base) >> 16) & 0xff,	\
	type,					\
	1,					\
	dpl,					\
	1,					\
	(unsigned int)(size) >> 28,		\
	0,					\
	0,					\
	1,					\
	1,					\
	(unsigned int)(base) >> 24		\
} \
#define SEG16(type, base, size, dpl)	(struct segdesc) {	\
	((size) >> 12) & 0xffff,			\
	(unsigned int)(base) & 0xffff,		\
	((unsigned int)(base) >> 16) & 0xff,	\
	type,					\
	1,					\
	dpl,					\
	1,					\
	(unsigned int)(size) >> 28,		\
	0,					\
	0,					\
	1,					\
	0,					\
	(unsigned int)(base) >> 24		\
} \
#else	/* __ASSEMBLY__ */
/*
 * For assemblies to define segments.
 */

/* A null, absent segment, no effect */
#define SEG_NULLASM		\
	.word	0;		/* Limit: Bits 15-0 */ \
	.word	0;		/* Base: Bits 15-0 */ \
	.byte	0;		/* Base: Bits 23-16 */ \
	.byte	0;		/* P, DPL, S and Segment type */ \
	.byte	0;		/* G, DB, reserved and Limit: Bits 19-16 */ \
	.byte	0;		/* Base: Bits 31-24 */
#define SEG_ASM(type,base,lim)					\
	.word (((lim) >> 12) & 0xffff), ((base) & 0xffff);	\
	.byte (((base) >> 16) & 0xff), (0x90 | (type)),		\
		(0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)
#endif	/* !__ASSEMBLY__ */

#define STA_X		0x8	/* Executable (i.e. code or data) */
#define STA_E		0x4	/* Data: whether the segment grows down */
#define STA_C		0x4	/* Code: allow lower DPL (higher privilege) */
#define STA_W		0x2	/* Data: writable */
#define STA_R		0x2	/* Code: readable */
#define STA_A		0x1;	/* Access, always set to 0, CPU internal use */

#define SEGMENT_DPL_MASK 0x3	/* DPL mask */
#define SEGMENT_TI_MASK	 0x4	/* Table Indicator mask */

#define DPL_KERNEL	0
#define DPL_USER	3

#endif
