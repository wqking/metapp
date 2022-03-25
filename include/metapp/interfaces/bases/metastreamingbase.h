// metapp library
// 
// Copyright (C) 2022 Wang Qi (wqking)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef METAPP_METASTREAMINGBASE_H_969872685611
#define METAPP_METASTREAMINGBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metastreaming.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/exception.h"

namespace metapp_top_internal_ {

// These functions calling streaming << or >> operators can't be in global,
// or metapp, or any nested namespace in metapp
// Before if they are inside metapp namespace, for type that can cast to other
// type implicitly, such as std::reference_wrapper, due to ADL, the compiler
// will choose both the operators, one is Variant, the other is the casted type.
// That will cause "use of overloaded operator '<<' is ambiguous" error.
// Putting these functions in metapp_top_internal_ will prevent ADL.

template <typename U>
inline void doStreamIn(std::istream & stream, metapp::Variant & value,
	typename std::enable_if<metapp::internal_::HasInputStreamOperator<U>::value >::type * = nullptr)
{
	using M = typename std::remove_reference<U>::type;
	stream >> *static_cast<M *>(value.getAddress());
}

template <typename U>
inline void doStreamIn(std::istream & /*stream*/, metapp::Variant & /*value*/,
	typename std::enable_if<! metapp::internal_::HasInputStreamOperator<U>::value >::type * = nullptr)
{
	errorUnsupported("No << input streaming operator.");
	return;
}

template <typename U>
inline void doStreamOut(std::ostream & stream, const metapp::Variant & value,
	typename std::enable_if<metapp::internal_::HasOutputStreamOperator<U>::value >::type * = nullptr)
{
	using M = typename std::remove_reference<U>::type;
	stream << *static_cast<const M *>(value.getAddress());
}

template <typename U>
inline void doStreamOut(std::ostream & /*stream*/, metapp::Variant & /*value*/,
	typename std::enable_if<! metapp::internal_::HasOutputStreamOperator<U>::value >::type * = nullptr)
{
	errorUnsupported("No >> output streaming operator.");
	return;
}

} // namespace metapp_top_internal_

namespace metapp {

template <typename T>
struct MetaStreamingBase
{
public:
	static const metapp::MetaStreaming * getMetaStreaming() {
		static metapp::MetaStreaming metaStreaming(
			&streamIn,
			&streamOut
		);
		return &metaStreaming;
	}

private:
	static void streamIn(std::istream & stream, metapp::Variant & value) {
		metapp_top_internal_::doStreamIn<T>(stream, value);
	}

	static void streamOut(std::ostream & stream, const metapp::Variant & value) {
		metapp_top_internal_::doStreamOut<T>(stream, value);
	}

};

namespace internal_ {

struct DummyMetaStreamingBase{};

} // namespace internal_

template <typename T>
using SelectMetaStreamingBase = typename std::conditional<
	internal_::HasInputStreamOperator<T>::value
		&& internal_::HasOutputStreamOperator<T>::value
	,
	MetaStreamingBase<T>,
	internal_::DummyMetaStreamingBase
>::type;


} // namespace metapp

#include "metapp/variant.h"

#endif

