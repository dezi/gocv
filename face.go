package gocv

/*
#include <stdlib.h>
#include "face.h"
*/
import "C"
import (
	"image"
	"reflect"
	"unsafe"
)

type Facemark struct {
	p C.Facemark
}

func NewFacemarkLBF() Facemark {
	return Facemark{p: C.FacemarkLBF_New()}
}

func (f *Facemark) Close() error {
	C.Facemark_Close(f.p)
	return nil
}

func (f *Facemark) LoadModel(name string) bool {
	cName := C.CString(name)
	defer C.free(unsafe.Pointer(cName))
	C.Facemark_LoadModel(f.p, cName)
	return true
}

func (f *Facemark) Fit(frame Mat, rects []image.Rectangle) (points [][]Point2f) {

	if len(rects) == 0 {
		return
	}

	cRects := toCRects(rects)
	ret := C.Facemark_Fit(f.p, frame.p, cRects)
	defer C.Contours2f_Close(ret)

	cArray := ret.contours
	cLength := int(ret.length)
	cHdr := reflect.SliceHeader{
		Data: uintptr(unsafe.Pointer(cArray)),
		Len:  cLength,
		Cap:  cLength,
	}
	sContours := *(*[]C.Points2f)(unsafe.Pointer(&cHdr))

	contours := make([][]Point2f, cLength)
	for i, pts := range sContours {
		pArray := pts.points
		pLength := int(pts.length)
		pHdr := reflect.SliceHeader{
			Data: uintptr(unsafe.Pointer(pArray)),
			Len:  pLength,
			Cap:  pLength,
		}
		sPoints := *(*[]C.Point2f)(unsafe.Pointer(&pHdr))

		points := make([]Point2f, pLength)
		for j, pt := range sPoints {
			points[j] = Point2f{float32(pt.x), float32(pt.y)}
		}
		contours[i] = points
	}

	return contours
}

func toCRects(rects []image.Rectangle) C.struct_Rects {

	cRectSlice := make([]C.struct_Rect, len(rects))

	for i, rect := range rects {
		cRectSlice[i] = C.struct_Rect{
			x:      C.int(rect.Min.X),
			y:      C.int(rect.Min.Y),
			width:  C.int(rect.Size().X),
			height: C.int(rect.Size().Y),
		}
	}

	return C.struct_Rects{
		rects:  (*C.Rect)(&cRectSlice[0]),
		length: C.int(len(rects)),
	}
}
