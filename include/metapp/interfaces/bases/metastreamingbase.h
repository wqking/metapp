#ifndef METAPP_METASTREAMINGBASE_H_969872685611
#define METAPP_METASTREAMINGBASE_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metastreaming.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/exception.h"

namespace metapp {

template <typename T>
struct MetaStreamingBase
{
public:
	static const MetaStreaming * getMetaStreaming() {
		static MetaStreaming metaStreaming(
			&streamIn,
			&streamOut
		);
		return &metaStreaming;
	}

private:
	static void streamIn(std::istream & stream, Variant & value) {
		doStreamIn<T>(stream, value);
	}

	static void streamOut(std::ostream & stream, const Variant & value) {
		doStreamOut<T>(stream, value);
	}

	template <typename U>
	static void doStreamIn(std::istream & stream, Variant & value,
		typename std::enable_if<internal_::HasInputStreamOperator<U>::value >::type * = 0) {
		using M = typename std::remove_reference<U>::type;
		stream >> *static_cast<M *>(value.getAddress());
	}

	template <typename U>
	static void doStreamIn(std::istream & /*stream*/, Variant & /*value*/,
		typename std::enable_if<! internal_::HasInputStreamOperator<U>::value >::type * = 0) {
		errorUnsupported("No << input streaming operator.");
		return;
	}

	template <typename U>
	static void doStreamOut(std::ostream & stream, const Variant & value,
		typename std::enable_if<internal_::HasOutputStreamOperator<U>::value >::type * = 0) {
		using M = typename std::remove_reference<U>::type;
		stream << *static_cast<const M *>(value.getAddress());
	}

	template <typename U>
	static void doStreamOut(std::ostream & /*stream*/, Variant & /*value*/,
		typename std::enable_if<! internal_::HasOutputStreamOperator<U>::value >::type * = 0) {
		errorUnsupported("No >> output streaming operator.");
		return;
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

