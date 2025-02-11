/**
  * Macroses and local definitions.
  *
  */


/**
  * @brief Register definitions.
  *
  */
tmpa    .req R0
tmpd    .req R1
zero    .req R10
one     .req R11
_OFF_   .req R10
_ON_    .req R11
tmp     .req R12


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
	mov tmpd, #\value
	str tmpd, [tmpa, #\offset]
.endm


/**
  * @brief Stores a HIGH HALF WORD "value" onto the given "addr" with an "offset". 
  */
.macro STRHHW value, addr, offset=0
	ldr tmpa, =\addr
	movt tmpd, #\value
	str tmpd, [tmpa, #\offset]
.endm


/**
  * @brief Stores a BYTE "value" onto the given "addr" with an "offset".
  */
.macro STRBW value, addr, offset=0
	ldr tmpa, =\addr
	mov tmpd, #\value
	strb tmpd, [tmpa, #\offset]
.endm



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
