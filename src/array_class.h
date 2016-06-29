/* Class(es) used for linear, strided and blockstrided input and output */

#ifndef ARRAY_CLASS_H
#define ARRAY_CLASS_H

// linear access, base class; not used in test
class ArrayAccess {
protected:
   double *array;
public:
   ArrayAccess(double *_array):
      array(_array) { }

   virtual double& operator[](int i) {
      return array[i];
   }
	virtual ArrayAccess operator+(int offset) {
		return ArrayAccess(array + offset);
	}
};

// strided access
class StridedArrayAccess : public ArrayAccess {
protected:
   int stride;
public:
   StridedArrayAccess(double *_array, int _stride) :
      ArrayAccess(_array), stride(_stride) { }

   double& operator[](int i) {
      return array[i*stride];
   }
	StridedArrayAccess operator+(int offset) {
		return StridedArrayAccess(array + offset * stride, stride);
	}
};

// blockstrided access
class BlockStridedArrayAccess : public StridedArrayAccess {
private:
   int block;
public:
   BlockStridedArrayAccess(double *_array, int _stride, int _block) :
      StridedArrayAccess(_array, _stride), block(_block) { }

   double& operator[](int i) {
      return array[(i/block)*stride + i%block];
   }
	BlockStridedArrayAccess operator+(int offset) {
      return BlockStridedArrayAccess(
            array + (offset/block*stride + offset%block),
            stride, block);
	}
};

#endif

