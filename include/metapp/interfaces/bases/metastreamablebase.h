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

#ifndef METAPP_METASTREAMABLEBASE_H_969872685611
#define METAPP_METASTREAMABLEBASE_H_969872685611

#include "metapp/implement/variant_intf.h"
#include "metapp/interfaces/metastreamable.h"
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
inline void doStreamIn(std::istream & stream, metapp::Variant & value, std::true_type)
{
	using M = typename std::remove_reference<U>::type;
	stream >> *static_cast<M *>(value.getAddress());
}

template <typename U>
inline void doStreamIn(std::istream & /*stream*/, metapp::Variant & /*value*/, std::false_type)
{
	metapp::raiseException<metapp::UnsupportedException>("No >> input streaming operator.");
}

template <typename U>
inline void doStreamOut(std::ostream & stream, const metapp::Variant & value, std::true_type)
{
	using M = typename std::remove_reference<U>::type;
	stream << *static_cast<const M *>(value.getAddress());
}

template <typename U>
inline void doStreamOut(std::ostream & /*stream*/, const metapp::Variant & /*value*/, std::false_type)
{
	metapp::raiseException<metapp::UnsupportedException>("No << output streaming operator.");
}

} // namespace metapp_top_internal_

namespace metapp {

template <typename T, typename Enabled = void>
struct MetaStreamableBase
{
};

template <typename T>
struct MetaStreamableBase <T, typename std::enable_if<
		! std::is_reference<T>::value
		&& (internal_::HasInputStreamOperator<T>::value
		|| internal_::HasOutputStreamOperator<T>::value)
	>::type>
{
public:
	static const metapp::MetaStreamable * getMetaStreamable() {
		static metapp::MetaStreamable metaStreamable(
			&streamIn,
			&streamOut
		);
		return &metaStreamable;
	}

private:
	static void streamIn(std::istream & stream, metapp::Variant & value) {
		metapp_top_internal_::doStreamIn<T>(
			stream,
			value,
			std::integral_constant<bool, internal_::HasInputStreamOperator<T>::value>()
		);
	}

	static void streamOut(std::ostream & stream, const metapp::Variant & value) {
		metapp_top_internal_::doStreamOut<T>(
			stream,
			value,
			std::integral_constant<bool, internal_::HasOutputStreamOperator<T>::value>()
		);
	}

};

} // namespace metapp

#include "metapp/metatype.h"

#include "metapp/implement/variant_impl.h"

#endif

