/*******************************************************************************
 * Copyright (c) 2012, Dougal J. Sutherland (dsutherl@cs.cmu.edu).             *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 *     * Redistributions of source code must retain the above copyright        *
 *       notice, this list of conditions and the following disclaimer.         *
 *                                                                             *
 *     * Redistributions in binary form must reproduce the above copyright     *
 *       notice, this list of conditions and the following disclaimer in the   *
 *       documentation and/or other materials provided with the distribution.  *
 *                                                                             *
 *     * Neither the name of Carnegie Mellon University nor the                *
 *       names of the contributors may be used to endorse or promote products  *
 *       derived from this software without specific prior written permission. *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   *
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     *
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     *
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  *
 * POSSIBILITY OF SUCH DAMAGE.                                                 *
 ******************************************************************************/
#ifndef KERNELS_HPP_
#define KERNELS_HPP_
#include "sdm/basics.hpp"

#include <string>

#include <np-divs/div-funcs/div_func.hpp>

namespace sdm {

// TODO this should be copyable, but need DivFunc to be first
class Kernel {
    protected:
        NPDivs::DivFunc &div_func;

    public:
        Kernel(NPDivs::DivFunc &div_func) : div_func(div_func) {}
        virtual ~Kernel() {}

        virtual std::string name() const = 0;

        NPDivs::DivFunc& getDivFunc() const { return div_func; }

        virtual double transformDivergence(double div) const = 0;

        virtual void transformDivergences(double* div, size_t n) const {
            for (size_t i = 0; i < n; i++)
                for (size_t j = 0; j < n; j++)
                    div[i*n + j] = this->transformDivergence(div[i*n + j]);
        }
};


class LinearKernel : public Kernel {
    typedef Kernel super;

    public:
        LinearKernel(NPDivs::DivFunc &div_func) : super(div_func) {}

        virtual std::string name() const {
            return "Linear(" + div_func.name() + ")";
        }

        virtual double transformDivergence(double div) const { return div; }
        virtual void transformDivergences(double* div, size_t n) const { }
};


class GaussianKernel : public Kernel {
    typedef Kernel super;

    protected:
        double sigma;

    public:
        GaussianKernel(NPDivs::DivFunc &div_func, double sigma)
            : super(div_func), sigma(sigma)
            { }

        virtual std::string name() const;

        virtual double transformDivergence(double div) const;
        // TODO implement vectorized gaussian kernel
};

}
#endif
