.segment "CODE"

.scope pretitle
  .scope PaletteFade
    .enum State
      fadeIn
      hold
      fadeOut
      transition
    .endenum

    animationState = $60
    paletteIndex = $61
    timer = $62

    .proc update_palettes
      Vram PALETTE
      lda paletteIndex
      asl
      asl
      tax
      lda presents_palettes, x
      sta PPU_DATA
      lda presents_palettes + 1, x
      sta PPU_DATA
      lda presents_palettes + 2, x
      sta PPU_DATA
      lda presents_palettes + 3, x
      sta PPU_DATA
      lda engineering_palettes, x
      sta PPU_DATA
      lda engineering_palettes + 1, x
      sta PPU_DATA
      lda engineering_palettes + 2, x
      sta PPU_DATA
      lda engineering_palettes + 3, x
      sta PPU_DATA
      lda ogc_palettes, x
      sta PPU_DATA
      lda ogc_palettes + 1, x
      sta PPU_DATA
      lda ogc_palettes + 2, x
      sta PPU_DATA
      lda ogc_palettes + 3, x
      sta PPU_DATA
      rts
    .endproc

    .proc set_animation_state
      sta animationState
      tax
      lda state_timers, x
      sta timer
      rts
    .endproc

    .proc set_title_state
      SetGameState #GameState::title
      rts
    .endproc

    .proc init
      lda #State::fadeIn
      jsr set_animation_state
      lda #0
      sta paletteIndex
      jsr update_palettes
      rts
    .endproc

    .proc update
      dec timer
      beq @animate
      rts
    @animate:
      lda animationState
      cmp #State::fadeIn
      beq @fadeIn
      cmp #State::hold
      beq @hold
      cmp #State::fadeOut
      beq @fadeOut
      cmp #State::transition
      beq @transition
    @fadeIn:
      inc paletteIndex
      lda paletteIndex
      cmp #3
      bne @reset_timer
      lda #State::hold
      jsr set_animation_state
      rts
    @hold:
      lda #State::fadeOut
      jsr set_animation_state
      rts
    @fadeOut:
      dec paletteIndex
      bne @reset_timer
      lda #State::transition
      jsr set_animation_state
      rts
    @transition:
      jsr set_title_state
      rts
    @reset_timer:
      ldx animationState
      lda state_timers, x
      sta timer
      rts
    .endproc

  state_timers:
    ; timers to control the length of time each fade or transition is on screen
    ; assumed to be fade in, hold, fade out, transtiion
    .byte 8, 140, 8, 42
  ogc_palettes:
    .byte $0F, $0F, $0F, $0F ; fade in
    .byte $0F, $06, $0F, $0F ; hold
    .byte $0F, $06, $0F, $0F ; fade out
    .byte $0F, $16, $06, $05 ; transition
  engineering_palettes:
    .byte $0F, $0F, $0F, $0F ; fade in
    .byte $0F, $10, $0D, $0F ; hold
    .byte $0F, $10, $1D, $0F ; fade out
    .byte $0F, $10, $2D, $00 ; transition
  presents_palettes:
    .byte $0F, $0F, $0F, $0F ; fade in
    .byte $0F, $0F, $0F, $02 ; hold
    .byte $0F, $0F, $0F, $12 ; fade out
    .byte $0F, $0F, $03, $32 ; transition
  .endscope

  .proc drawOGCEngineeringLogo
    ; Draw starting on nametable block 4, 12, offset $10
    ; pattern pulled from pattern table 2 starting at offset $10
    ; ogcengineering.bin start with $18 $03 and then increments from $00 to $47
    ; 72 blocks sequentially in pattern table represent image of interest
    ; $10 - $27 - 24 blocks
    ; $28 - $3F - 24 blocks
    ; $40 - $57 - 24 blocks
    ; 72 blocks total
    ; $18 represents the width in blocks and $03 represents the height in blocks
    ; the next 72 bytes define the offset from the pattern table above the passed in starting offset to pull a pattern from
    ; the blocks are filled in from top left to bottom right according the .bin file limits
    
    DrawImage image_ogcengineering, 6, 12, $00
    ; draw text on background
    DrawText 12, 16, str_presents, NAMETABLE_A
    ; draw text on background
    DrawText 5, 26, str_email, NAMETABLE_A
    FillAttributes attr_ogcengineering
    rts
  .endproc

  .proc init
    lda #%10010000
    sta PPU_CTRL
    jsr clear_screen
    jsr PaletteFade::init
    jsr drawOGCEngineeringLogo
    rts
  .endproc

  .proc draw
    jsr PaletteFade::update_palettes
    VramReset
    rts
  .endproc

  .proc game_loop
    jsr PaletteFade::update
    rts
  .endproc

  ; draw the neshacker logo -> ogcengineering logo
  ; hex analysis simply shows this as some config data followed by incrementing from $00 to $47 ?
  ; perhaps each tile was placed as needed to build the image and this simply goes through the pattern table in sequence
  ; lets reverse it
  image_ogcengineering:  .incbin "./src/bin/ogcengineering.bin"
  ; attr table values ?(image width,2,image height,4,5,6,7,8,9)
  attr_ogcengineering:   .byte 19, 0, 0, %01010101, 5, %10101010, 8, %00000000, 0
  ; write the presents string using pattern table patterns, each of these bytes is a pointer to a tile in pattern table 2
  ; $58 points to 'P', I changed $5C to $5A in the 5th byte to reuse the 'E' but it is in fact in the pattern table twice using more memory than necessary
  ; I'm not sure about the last byte '0'... possibly a terminator ( null terminated string )
  ; "PRESENTS" referencing pattern table 2 blocks
  str_presents:     .byte 'p', 'r', 'e', 's', 'e', 'n', 't', 's', 0

  ; "DUSTIN@OGC.ENGINEERING" referencing pattern table 1 standard ascii offsets...
  str_email:        .byte 'd', 'u', 's', 't', 'i', 'n', '@', 'o', 'g', 'c', '.', 'e', 'n', 'g', 'i', 'n', 'e', 'e', 'r', 'i', 'n', 'g', 0
.endscope
