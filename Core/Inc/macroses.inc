/**
  * Macroses.
  *
  */

.include "local.inc"

/** ------------------------------------------------------------------------- */
/** WORD operations. */

/**
  * @brief Stores a WORD "value" onto the given "addr" with an "offset". 
  */
.macro STRW value, addr, offset=0
	ldr tmpa, =\addr
	ldr tmpd, =\value
	str tmpd, [tmpa, #\offset]
.endm


/**
  * @brief Stores a LOW HALF WORD "value" onto the given "addr" with an "offset". 
  */
.macro STRLHW value, addr, offset=0
	ldr tmpa, =\addr
	mov tmpd, \value
	str tmpd, [tmpa, #\offset]
.endm


/**
  * @brief Stores a HIGH HALF WORD "value" onto the given "addr" with an "offset". 
  */
.macro STRHHW value, addr, offset=0
	ldr tmpa, =\addr
	movt tmpd, \value
	str tmpd, [tmpa, #\offset]
.endm


/**
  * @brief Stores a BYTE "value" onto the given "addr" with an "offset".
  */
.macro STRBW value, addr, offset=0
	ldr tmpa, =\addr
	mov tmpd, \value
	strb tmpd, [tmpa, #\offset]
.endm



/** ------------------------------------------------------------------------- */
/** In case if a value is already loaded into a register "value". */
 
/**
  * @brief Stores a WORD "value" onto the given "addr" with an "offset". 
  */
.macro STRWG value, addr, offset=0
	ldr tmpa, =\addr
	str \value, [tmpa, #\offset]
.endm


/**
  * @brief Stores a HIGH HALF WORD "value" onto the given "addr" with an "offset". 
  */
.macro STRHG value, addr, offset=0
	ldr tmpa, =\addr
	strh \value, [tmpa, #\offset]
.endm


/**
  * @brief Stores a BYTE "value" onto the given "addr" with an "offset".
  */
.macro STRBG value, addr, offset=0
	ldr tmpa, =\addr
	strb \value, [tmpa, #\offset]
.endm



/** ------------------------------------------------------------------------- */

/**
  * @brief Loads a WORD "value" from the given "addr" with an "offset". 
  */
.macro LDRW value, addr, offset=0
	ldr tmpa, =\addr
	ldr \value, [tmpa, #\offset]
.endm

 
/**
  * @brief Loads a BYTE "value" from the given "addr" with an "offset". 
  */
.macro LDRBW value, addr, offset=0
	ldr tmpa, =\addr
	ldrb \value, [tmpa, #\offset]
.endm

 
/**
  * @brief Loads a WORD "value" from the given PC-relative "addr" with an "offset". 
  */
.macro LDRA value, addr, offset=0
	adr tmpa, \addr
	ldr \value, [tmpa, #\offset]
.endm
	
 
/**
  * @brief Loads a BYTE "value" from the given PC-relative "addr" with an "offset". 
  */
.macro LDRBA value, addr, offset=0
	adr tmpa, \addr
	ldrb \value, [tmpa, #\offset]
.endm



/** ------------------------------------------------------------------------- */

/**
  * @brief Transfer a WORD value from the source address "srcaddr" with a source offset
  *        to the destination address "dstaddr" with a destination offset.
  * 
  */
.macro MOVV srcaddr, srcoffset=0, dstaddr, dstoffset=0
	ldr tmpa, =\srcaddr
	ldr tmpd, [tmpa, #\srcoffset]
	ldr tmpa, =\dstaddr
	str tmpd, [tmpa, #\dstoffset]
.endm
	

/**
  * @brief Transfer a WORD value from the source address "srcaddr" with a source offset
  *        to the destination address "dstaddr" with a destination offset. The offset 
  *        values have to be stored to specified registers before calling the macros.
  * 
  */
.macro MOVVG srcaddr, srcoffset, dstaddr, dstoffset
	ldr tmpa, =\srcaddr
	ldr tmpd, [tmpa, #\srcoffset]
	ldr tmpa, =\dstaddr
	str tmpd, [tmpa, #\dstoffset]
.endm


/**
  * @brief Transfer a BYTE value from the source address "srcaddr" with a source offset
  *        to the destination address "dstaddr" with a destination offset.
  * 
  */
.macro MOVVB srcaddr, srcoffset=0, dstaddr, dstoffset=0
	ldr tmpa, =\srcaddr
	ldrb tmpd, [tmpa, #\srcoffset]
	ldr tmpa, =\dstaddr
	strb tmpd, [tmpa, #\dstoffset]
.endm


/**
  * @brief Transfer a BYTE value from the source address "srcaddr" with a source offset
  *        to the destination address "dstaddr" with a destination offset. The offset 
  *        values have to be stored to specified registers before calling the macros.
  * 
  */
.macro MOVVBG srcaddr, srcoffset, dstaddr, dstoffset
	ldr tmpa, =\srcaddr
	ldrb tmpd, [tmpa, #\srcoffset]
	ldr tmpa, =\dstaddr
	strb tmpd, [tmpa, #\dstoffset]
.endm



/** ------------------------------------------------------------------------- */
/**  Peripherals operations */

/**
  * @brief Loads the given peripheral bit address into "tmpa" (temporary address) register.
  */
.macro MOVP paddr
	ldr tmpa, =((\paddr & 0x00ffffff) * 0x20 + PERIPH_BB)
.endm


/**
  * @brief Loads the given peripheral value into a register.
  *
  */
.macro LDRP value, offset=0
  ldr \value, [tmpa, #(\offset * 4)] 
.endm


/**
  * @brief Stores the given value into the peripheral address that located in 
  *        "tmpa" (temporary address) register.
  *
  */
.macro STRP value, offset=0
  str \value, [tmpa, #(\offset * 4)] 
.endm


/**
  * @brief Loads the given BYTE value into peripheral address that located in 
  *        "tmpa" (temporary address) register.
  *
  */
.macro LDRPB value, paddr, offset=0
  MOVP \paddr
  LDRP \value, \offset
  @ ldr tmpa, =((\paddr & 0x00ffffff) * 0x20 + PERIPH_BB)
  @ ldr \value, [tmpa, #(\offset * 4)] 
.endm


/**
  * @brief Stores the given BYTE value into peripheral address that located in 
  *        "tmpa" (temporary address) register.
  *
  */
.macro STRPB value, paddr, offset=0
  mov tmpd, \value
  MOVP \paddr
  STRP tmpd, \offset
  @ ldr tmpa, =((\paddr & 0x00ffffff) * 0x20 + PERIPH_BB)
  @ str \value, [tmpa, #(\offset * 4)] 
.endm



/** ------------------------------------------------------------------------- */
/**  SRAM operations */

/**
  * @brief Loads the value bit from the given SRAM address.
  *
  */
.macro MOVR addr
  ldr tmpa, =((\addr & 0x0000ffff) * 0x20 + SRAM_BB)
.endm


/**
  * @brief Loads the given SRAM value into a register.
  *
  */
.macro LDRR value, offset=0
	ldr \value, [tmpa, #(\offset * 4)] 
.endm


/**
  * @brief Stores the given SRAM value into a register.
  *
  */
.macro STRR value, offset=0
	str \value, [tmpa, #(\offset * 4)] 
.endm


/**
  * @brief Loads the value bit from the given SRAM address.
  *
  */
.macro LDRRB value, addr, offset=0
  MOVR \addr
  LDRR \value, \offset
	@ ldr tmpa, =((\addr & 0x0000ffff) * 0x20 + SRAM_BB)
	@ ldr \value, [tmpa, #(\offset * 4)] 
.endm


/**
  * @brief Stores the value bit from the given SRAM address.
  *
  */
.macro STRRB value, addr, offset=0
  MOVR \addr
  mov tmp, \value
  STRR tmp, \offset
	@ ldr tmpa, =((\addr & 0x0000ffff) * 0x20 + SRAM_BB)
	@ str \value, [tmpa, #(\offset * 4)] 
.endm



/** ------------------------------------------------------------------------- */
/** Transfwr data from Flash to SRAM */

/**
  * @brief Transfers an immediate number of WORD values from the source address to the destination address.
  * 
  */
.macro MOVVA srcaddr, dstaddr, nw 
	push {v1-v4}
	ldr v1, =(\dstaddr)
	ldr v2, =(\srcaddr)
	ldr v3, =\nw

.set GCNT = GCNT + 1
	
_LOAD_MOVVA_cycleGCNT:
	ldrne v4, [v2, #4]!
	strne v4, [v1, #4]!
	sub v3, 1
	teq v3, 0
	bne _LOAD_MOVVA_cycleGCNT

	pop {v1-v4}
.endm


/**
  * @brief Transfers the number of WORD values (allocated in a register) from the source address to the destination address.
  * 
  */
.macro MOVVAG srcaddr, dstaddr, nw
  push {v1-v4}
	ldr v1, =(\dstaddr - 4)
	ldr v2, =(\srcaddr - 4)
	add v3, \nw, 1

.set GCNT = GCNT + 1
	
_LOAD_MOVVAG_cycleGCNT:
	ldrne v4, [v2, #4]!
	strne v4, [v1, #4]!
	sub v3, #1
	teq v3, #0
	bne _LOAD_MOVVAG_cycleGCNT

	pop {v1-v4}
.endm



/** ------------------------------------------------------------------------- */
/** Status bit operations */

/** 
  * @brief Tests the status bit in the given peripheral.
  *
  */
.macro TSTPB peripheral, offset=0
  LDRPB tmpd, \peripheral, \offset
	tst tmpd, 1
.endm


/** 
  * @brief Tests the given status bit of the value from the given address.
  *
  */
.macro TSTRB addr, offset=0
  LDRRB tmpd, \addr, \offset
	tst tmpd, 1
.endm



/** ------------------------------------------------------------------------- */
/** Routine flag operations */

/**
  * @brief Sets the given flag the given register.
  *
  */
.macro FLS register, flag	
	STRRB 1, \register, \flag
.endm


/**
  * @brief Clears the given flag the given register.
  *
  */
.macro FLC register, flag	
	STRRB 0, \register, \flag
.endm


/**
  * @brief Tests the given flag the given register.
  *
  */
.macro FLT register, flag	
	TSTRB \register, \flag
.endm


/**
  * @brief Sets the given flag the given peripheral.
  *
  */
.macro FLSP periph, flag	
	STRPB 1, \periph, \flag
.endm


/**
  * @brief Clears the given flag the given peripheral.
  *
  */
.macro FLCP periph, flag	
	STRPB 0, \periph, \flag
.endm


/**
  * @brief Tests the given flag the given peripheral.
  *
  */
.macro FLTP periph, flag	
	TSTPB \periph, \flag
.endm
	


/** ------------------------------------------------------------------------- */
/** Program status register (PSR) operations */

/**
  * @brief Tests the given flag in the program status register (PSR).
  *
  */
.macro APSRC flag
	push {tmp}
	mrs tmp, APSR
	bic tmp, tmp, (1<<\flag)
	msr APSR_nzcvq, tmp
	pop {tmp}
.endm
