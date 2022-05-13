//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// quat.template.h file to make changes.

#ifndef PXR_BASE_GF_{{ UPPER(QUAT)[2:] }}_H
#define PXR_BASE_GF_{{ UPPER(QUAT)[2:] }}_H

/// \file gf/quat{{ SUFFIX }}.h
/// \ingroup group_gf_LinearAlgebra

#include "pxr/pxr.h"
#include "pxr/base/gf/api.h"
#include "pxr/base/gf/declare.h"
#include "pxr/base/gf/vec3{{ SUFFIX }}.h"
#include "pxr/base/gf/traits.h"
{% if SCL == 'GfHalf' -%}
#include "pxr/base/gf/half.h"
{% endif %}

#include <boost/functional/hash.hpp>

#include <iosfwd>

PXR_NAMESPACE_OPEN_SCOPE

template <>
struct GfIsGfQuat<class {{ QUAT }}> { static const bool value = true; };


/// Return the dot (inner) product of two quaternions.
{{ SCL }} GfDot(const {{ QUAT }}& q1, const {{ QUAT }}& q2);


/// \class {{ QUAT }}
/// \ingroup group_gf_LinearAlgebra
///
/// Basic type: a quaternion, a complex number with a real coefficient and
/// three imaginary coefficients, stored as a 3-vector.
///
class {{ QUAT }}
{
  public:
    typedef {{ SCL }} ScalarType;
    typedef GfVec3{{ SUFFIX }} ImaginaryType;

    /// Default constructor leaves the quaternion undefined.
    {{ QUAT }}() {}

    /// Initialize the real coefficient to \p realVal and the imaginary
    /// coefficients to zero.
    ///
    /// Since quaternions typically must be normalized, reasonable values for
    /// \p realVal are -1, 0, or 1.  Other values are legal but are likely to
    /// be meaningless.
    ///
    explicit {{ QUAT }} ({{ SCL }} realVal) : _imaginary(0), _real(realVal) {}

    /// Initialize the real and imaginary coefficients.
    {{ QUAT }}({{ SCL }} real, {{ SCL }} i, {{ SCL }} j, {{ SCL }} k)
        : _imaginary(i, j, k), _real(real)
    {
    }

    /// Initialize the real and imaginary coefficients.
    {{ QUAT }}({{ SCL }} real, const GfVec3{{ SUFFIX }} &imaginary)
        : _imaginary(imaginary), _real(real)
    {
    }

{% for S in SCALARS if S != SCL %}
    /// {{ "Implicitly convert" if ALLOW_IMPLICIT_CONVERSION(S, SCL) else "Construct" }} from {{ QUATNAME(S) }}.
    GF_API
    {{ '' if ALLOW_IMPLICIT_CONVERSION(S, SCL) else 'explicit ' }}{{ QUAT }}(class {{ QUATNAME(S) }} const &other);
{% endfor %}

    /// Return the zero quaternion, with real coefficient 0 and an
    /// imaginary coefficients all zero.
    static {{ QUAT }} GetZero() { return {{ QUAT }}(0.0); }

    /// Return the identity quaternion, with real coefficient 1 and an
    /// imaginary coefficients all zero.
    static {{ QUAT }} GetIdentity() { return {{ QUAT }}(1.0); }

    /// Return the real coefficient.
    {{ SCL }} GetReal() const { return _real; }

    /// Set the real coefficient.
    void SetReal({{ SCL }} real) { _real = real; }

    /// Return the imaginary coefficient.
    const GfVec3{{ SUFFIX }} &GetImaginary() const { return _imaginary; }

    /// Set the imaginary coefficients.
    void SetImaginary(const GfVec3{{ SUFFIX }} &imaginary) {
        _imaginary = imaginary;
    }

    /// Set the imaginary coefficients.
    void SetImaginary({{ SCL }} i, {{ SCL }} j, {{ SCL }} k) {
        _imaginary.Set(i, j, k);
    }

    /// Return geometric length of this quaternion.
    {{ SCL }} GetLength() const { return GfSqrt(_GetLengthSquared()); }

    /// length of this quaternion is smaller than \p eps, return the identity
    /// quaternion.
    {{ QUAT }}
    GetNormalized({{ SCL }} eps = GF_MIN_VECTOR_LENGTH) const {
        {{ QUAT }} ret(*this);
        ret.Normalize(eps);
        return ret;
    }

    /// Normalizes this quaternion in place to unit length, returning the
    /// length before normalization. If the length of this quaternion is
    /// smaller than \p eps, this sets the quaternion to identity.
    GF_API
    {{ SCL }} Normalize({{ SCL }} eps = GF_MIN_VECTOR_LENGTH);

    /// Return this quaternion's conjugate, which is the quaternion with the
    /// same real coefficient and negated imaginary coefficients.
    {{ QUAT }} GetConjugate() const {
        return {{ QUAT }}(GetReal(), -GetImaginary());
    }

    /// Return this quaternion's inverse, or reciprocal.  This is the
    /// quaternion's conjugate divided by it's squared length.
    {{ QUAT }} GetInverse() const {
        return GetConjugate() / _GetLengthSquared();
    }

    /// Transform the GfVec3{{ SUFFIX }} point. If the quaternion is normalized,
    /// the transformation is a rotation. Given a {{ QUAT }} q, q.Transform(point)
    /// is equivalent to:
    ///
    ///     (q * {{ QUAT }}(0, point) * q.GetInverse()).GetImaginary()
    ///
    /// but is more efficient.
    GF_API
    GfVec3{{ SUFFIX }} Transform(const GfVec3{{ SUFFIX }}& point) const;

    /// Hash.
    friend inline size_t hash_value(const {{ QUAT }} &q) {
        size_t h = boost::hash<ScalarType>()(q.GetReal());
        boost::hash_combine(h, q.GetImaginary());
        return h;
    }

    /// Component-wise negation.
    {{ QUAT }} operator-() const {
        return {{ QUAT }}(-GetReal(), -GetImaginary());
    }

    /// Component-wise quaternion equality test. The real and imaginary parts
    /// must match exactly for quaternions to be considered equal.
    bool operator==(const {{ QUAT }} &q) const {
        return (GetReal() == q.GetReal() &&
                GetImaginary() == q.GetImaginary());
    }

    /// Component-wise quaternion inequality test. The real and imaginary
    /// parts must match exactly for quaternions to be considered equal.
    bool operator!=(const {{ QUAT }} &q) const {
        return !(*this == q);
    }

    /// Post-multiply quaternion \p q into this quaternion.
    GF_API
    {{ QUAT }} &operator *=(const {{ QUAT }} &q);

    /// Multiply this quaternion's coefficients by \p s.
    {{ QUAT }} &operator *=({{ SCL }} s) {
        _real *= s;
        _imaginary *= s;
        return *this;
    }

    /// Divide this quaternion's coefficients by \p s.
    {{ QUAT }} &operator /=({{ SCL }} s) {
        _real /= s;
        _imaginary /= s;
        return *this;
    }

    /// Add quaternion \p q to this quaternion.
    {{ QUAT }} &operator +=(const {{ QUAT }} &q) {
        _real += q._real;
        _imaginary += q._imaginary;
        return *this;
    }

    /// Component-wise unary difference operator.
    {{ QUAT }} &operator -=(const {{ QUAT }} &q)  {
        _real -= q._real;
        _imaginary -= q._imaginary;
        return *this;
    }

    /// Component-wise binary sum operator.
    friend {{ QUAT }}
    operator +(const {{ QUAT }} &q1, const {{ QUAT }} &q2) {
        return {{ QUAT }}(q1) += q2;
    }

    /// Component-wise binary difference operator.
    friend {{ QUAT }}
    operator -(const {{ QUAT }} &q1, const {{ QUAT }} &q2) {
        return {{ QUAT }}(q1) -= q2;
    }

    /// Returns the product of quaternions \p q1 and \p q2.
    friend {{ QUAT }}
    operator *(const {{ QUAT }} &q1, const {{ QUAT }} &q2) {
        return {{ QUAT }}(q1) *= q2;
    }

    /// Returns the product of quaternion \p q and scalar \p s.
    friend {{ QUAT }}
    operator *(const {{ QUAT }} &q, {{ SCL }} s) {
        return {{ QUAT }}(q) *= s;
    }

    /// Returns the product of quaternion \p q and scalar \p s.
    friend {{ QUAT }}
    operator *({{ SCL }} s, const {{ QUAT }} &q) {
        return {{ QUAT }}(q) *= s;
    }

    /// Returns the product of quaternion \p q and scalar 1 / \p s.
    friend {{ QUAT }}
    operator /(const {{ QUAT }} &q, {{ SCL }} s) {
        return {{ QUAT }}(q) /= s;
    }

  private:
    /// Imaginary part
    GfVec3{{ SUFFIX }} _imaginary;

    /// Real part
    {{ SCL }} _real;

    /// Returns the square of the length
    {{ SCL }}
    _GetLengthSquared() const {
        return GfDot(*this, *this);
    }
};

/// Spherically linearly interpolate between \p q0 and \p q1.
///
/// If the interpolant \p alpha is zero, then the result is \p q0, while
/// \p alpha of one yields \p q1.
GF_API {{ QUAT }}
GfSlerp(double alpha, const {{ QUAT }}& q0, const {{ QUAT }}& q1);

GF_API {{ QUAT }}
GfSlerp(const {{ QUAT }}& q0, const {{ QUAT }}& q1, double alpha);

inline {{ SCL }}
GfDot({{ QUAT }} const &q1, {{ QUAT }} const &q2) {
    return GfDot(q1.GetImaginary(), q2.GetImaginary()) +
                 q1.GetReal()*q2.GetReal();
}

/// Output a GfQuatd using the format (re, i, j, k)
/// \ingroup group_gf_DebuggingOutput
GF_API std::ostream& operator<<(std::ostream &, {{ QUAT }} const &);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_BASE_GF_{{ UPPER(QUAT)[2:] }}_H
