// pybind/matrix/sparse_matrix_pybind.cc

// Copyright 2019   Mobvoi AI Lab, Beijing, China (author: Fangjun Kuang)

// See ../../../COPYING for clarification regarding multiple authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
// WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache 2 License for the specific language governing permissions and
// limitations under the License.

#include "matrix/sparse_matrix_pybind.h"

#include "matrix/sparse-matrix.h"

using namespace kaldi;

// in nnet-example.h, class NnetIO contains a field `GeneralMatrix features`
// so we need to wrap GeneralMatrix

void pybind_sparse_matrix(py::module& m) {
  {
    using PyClass = SparseMatrix<BaseFloat>;
    py::class_<PyClass>(m, "SparseMatrix",
                        "This class is defined for sparse matrix type.")
        .def(py::init<>())
        .def(py::init<const MatrixBase<BaseFloat>&>(), py::arg("mat"))
        .def(py::init<int32, int32>(), py::arg("num_rows"), py::arg("num_cols"))
        .def("NumRows", &PyClass::NumRows, "Return number of rows")
        .def("NumCols", &PyClass::NumCols, "Return number of columns")
        .def("NumElements", &PyClass::NumElements, "Return number of elements");
  }
  {
    using PyClass = GeneralMatrix;
    py::class_<PyClass>(
        m, "GeneralMatrix",
        "This class is a wrapper that enables you to store a matrix in one of "
        "three forms: either as a Matrix<BaseFloat>, or a CompressedMatrix, or "
        "a SparseMatrix<BaseFloat>.  It handles the I/O for you, i.e. you read "
        "and write a single object type.  It is useful for neural-net training "
        "targets which might be sparse or not, and might be compressed or not.")
        .def(py::init<>())
        .def(py::init<const MatrixBase<BaseFloat>&>(), py::arg("mat"))
        .def("GetMatrix", &PyClass::GetMatrix,
             "Outputs the contents as a matrix returns kFullMatrix, this will "
             "work regardless of Type().",
             py::arg("mat"));
  }
}
