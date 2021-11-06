import SwiftIO
import SwiftIOBoard
import MadARM2D
import ST7789


func draw_bitmap(x: Int32, y: Int32, width: Int32, height: Int32,  data:  UnsafeMutablePointer<UInt8>?) -> Int32 {
    let buffer = UnsafeBufferPointer(start: data!, count: Int(width * height * 2))
    screen.writeBitmap(x: Int(x), y: Int(y), width: Int(width), height: Int(height), data: Array(buffer))
    return 0
}

func draw_bg(tile: UnsafeMutableRawPointer?, target: UnsafeMutableRawPointer?) -> Int32 {
    return 0;
}

print("Hello, world!")

let white: UInt16 = 0xFFFF
let spi = SPI(Id.SPI0, speed: 50_000_000)
let cs = DigitalOut(Id.D9)
let dc = DigitalOut(Id.D1)
let rst = DigitalOut(Id.D0)
let bl = DigitalOut(Id.D4)
let screen = ST7789(spi: spi, cs: cs, dc: dc, rst: rst, bl: bl, rotation: .angle90)
screen.clearScreen(white)
sleep(ms: 1000)

mad_arm_2d_init(240,240,0,240,240,draw_bitmap,draw_bg)