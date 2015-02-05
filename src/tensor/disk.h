#if !defined(TENSOR_DISK_H)
#define TENSOR_DISK_H

#include "tensorimpl.h"

namespace tensor {

class DiskTensorImpl : public TensorImpl
{
public:
    DiskTensorImpl(const std::string& name, const Dimension& dims);
    ~DiskTensorImpl();

    void scale(
        double beta = 0.0);

    void slice(
        ConstTensorImplPtr A,
        const IndexRange& Cinds,
        const IndexRange& Ainds,
        double alpha = 1.0,
        double beta = 0.0);

    void permute(
        ConstTensorImplPtr A,
        const std::vector<std::string>& Cinds,
        const std::vector<std::string>& Ainds,
        double alpha = 1.0,
        double beta = 0.0);

private:
    std::string filename_;
    FILE* fh_;
};

typedef DiskTensorImpl* DiskTensorImplPtr;
typedef const DiskTensorImpl* ConstDiskTensorImplPtr;

}

#endif
