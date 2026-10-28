package main

import (
	"bytes"
	"encoding/binary"
	"image"
	"image/color"
	"math"
	"os"
	"path/filepath"
)

type pixel struct {
	r, g, b, a float64
}

type polygon struct {
	points [][2]float64
	alpha  float64
}

var plane = []polygon{
	{points: [][2]float64{{0.13, 0.44}, {0.87, 0.12}, {0.52, 0.62}}, alpha: 1},
	{points: [][2]float64{{0.52, 0.62}, {0.87, 0.12}, {0.72, 0.86}}, alpha: 0.84},
	{points: [][2]float64{{0.13, 0.44}, {0.52, 0.62}, {0.44, 0.80}}, alpha: 1},
}

func main() {
	outDir := "assets"
	if len(os.Args) > 1 {
		outDir = os.Args[1]
	}

	sizes := []int{16, 20, 24, 32, 40, 48, 64, 128, 256}
	images := make([][]byte, 0, len(sizes))
	for _, size := range sizes {
		images = append(images, encodeBMP(render(size)))
	}

	if err := os.WriteFile(filepath.Join(outDir, "icon.ico"), encodeICO(sizes, images), 0o644); err != nil {
		panic(err)
	}
}

func render(size int) *image.NRGBA {
	const ss = 8

	big := size * ss
	buf := make([]pixel, big*big)

	radius := 0.48
	top := [3]float64{42, 171, 238}
	bottom := [3]float64{28, 140, 190}

	for y := 0; y < big; y++ {
		for x := 0; x < big; x++ {
			cx := (float64(x) + 0.5) / float64(big)
			cy := (float64(y) + 0.5) / float64(big)
			if math.Hypot(cx-0.5, cy-0.5) > radius {
				continue
			}

			t := clamp((cy-(0.5-radius))/(2*radius), 0, 1)
			buf[y*big+x] = pixel{
				r: lerp(top[0], bottom[0], t) / 255,
				g: lerp(top[1], bottom[1], t) / 255,
				b: lerp(top[2], bottom[2], t) / 255,
				a: 1,
			}
		}
	}

	for _, shape := range plane {
		for y := 0; y < big; y++ {
			for x := 0; x < big; x++ {
				i := y*big + x
				if buf[i].a == 0 {
					continue
				}

				px := (float64(x) + 0.5) / float64(big)
				py := (float64(y) + 0.5) / float64(big)
				if !inside(shape.points, px, py) {
					continue
				}

				a := shape.alpha
				buf[i].r = buf[i].r*(1-a) + a
				buf[i].g = buf[i].g*(1-a) + a
				buf[i].b = buf[i].b*(1-a) + a
			}
		}
	}

	out := image.NewNRGBA(image.Rect(0, 0, size, size))
	for y := 0; y < size; y++ {
		for x := 0; x < size; x++ {
			var r, g, b, a float64
			for sy := 0; sy < ss; sy++ {
				for sx := 0; sx < ss; sx++ {
					p := buf[(y*ss+sy)*big+x*ss+sx]
					r += p.r * p.a
					g += p.g * p.a
					b += p.b * p.a
					a += p.a
				}
			}

			if a == 0 {
				continue
			}

			n := float64(ss * ss)
			out.SetNRGBA(x, y, color.NRGBA{
				R: uint8(math.Round(clamp(r/a, 0, 1) * 255)),
				G: uint8(math.Round(clamp(g/a, 0, 1) * 255)),
				B: uint8(math.Round(clamp(b/a, 0, 1) * 255)),
				A: uint8(math.Round(clamp(a/n, 0, 1) * 255)),
			})
		}
	}

	return out
}

func inside(points [][2]float64, x, y float64) bool {
	result := false
	j := len(points) - 1

	for i := range points {
		xi, yi := points[i][0], points[i][1]
		xj, yj := points[j][0], points[j][1]

		if (yi > y) != (yj > y) && x < (xj-xi)*(y-yi)/(yj-yi)+xi {
			result = !result
		}
		j = i
	}

	return result
}

func encodeBMP(img *image.NRGBA) []byte {
	w := img.Bounds().Dx()
	h := img.Bounds().Dy()

	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, uint32(40))
	binary.Write(&buf, binary.LittleEndian, int32(w))
	binary.Write(&buf, binary.LittleEndian, int32(h*2))
	binary.Write(&buf, binary.LittleEndian, uint16(1))
	binary.Write(&buf, binary.LittleEndian, uint16(32))
	binary.Write(&buf, binary.LittleEndian, uint32(0))
	binary.Write(&buf, binary.LittleEndian, uint32(w*h*4))
	binary.Write(&buf, binary.LittleEndian, int32(0))
	binary.Write(&buf, binary.LittleEndian, int32(0))
	binary.Write(&buf, binary.LittleEndian, uint32(0))
	binary.Write(&buf, binary.LittleEndian, uint32(0))

	for y := h - 1; y >= 0; y-- {
		for x := 0; x < w; x++ {
			c := img.NRGBAAt(x, y)
			buf.Write([]byte{c.B, c.G, c.R, c.A})
		}
	}

	maskRow := ((w + 31) / 32) * 4
	mask := make([]byte, maskRow*h)
	for y := 0; y < h; y++ {
		for x := 0; x < w; x++ {
			if img.NRGBAAt(x, h-1-y).A == 0 {
				mask[y*maskRow+x/8] |= 1 << (7 - uint(x%8))
			}
		}
	}
	buf.Write(mask)

	return buf.Bytes()
}

func encodeICO(sizes []int, images [][]byte) []byte {
	var buf bytes.Buffer
	binary.Write(&buf, binary.LittleEndian, uint16(0))
	binary.Write(&buf, binary.LittleEndian, uint16(1))
	binary.Write(&buf, binary.LittleEndian, uint16(len(images)))

	offset := 6 + 16*len(images)
	for i, data := range images {
		w := byte(sizes[i])
		h := byte(sizes[i])
		if sizes[i] == 256 {
			w = 0
			h = 0
		}

		buf.WriteByte(w)
		buf.WriteByte(h)
		buf.WriteByte(0)
		buf.WriteByte(0)
		binary.Write(&buf, binary.LittleEndian, uint16(1))
		binary.Write(&buf, binary.LittleEndian, uint16(32))
		binary.Write(&buf, binary.LittleEndian, uint32(len(data)))
		binary.Write(&buf, binary.LittleEndian, uint32(offset))
		offset += len(data)
	}

	for _, data := range images {
		buf.Write(data)
	}

	return buf.Bytes()
}

func lerp(a, b, t float64) float64 {
	return a + (b-a)*t
}

func clamp(v, lo, hi float64) float64 {
	return math.Min(math.Max(v, lo), hi)
}
