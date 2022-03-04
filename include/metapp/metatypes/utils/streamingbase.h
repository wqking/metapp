#ifndef METAPP_STREAMINGBASE_FUNCTION_H_969872685611
#define METAPP_STREAMINGBASE_FUNCTION_H_969872685611

#include "metapp/metatype.h"
#include "metapp/interfaces/metastreaming.h"
#include "metapp/implement/internal/typeutil_i.h"
#include "metapp/exception.h"
#include "metapp/variant.h"

namespace metapp {

template <typename T>
struct StreamingBase
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
		errorNotSupported("No << input streaming operator.");
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
		errorNotSupported("No >> output streaming operator.");
	}

};

namespace internal_ {
	struct DummyStreamingBase{};
} // namespace internal_

template <typename T>
using SelectStreamingBase = typename std::conditional<
	internal_::HasInputStreamOperator<T>::value
		| internal_::HasOutputStreamOperator<T>::value
	,
	StreamingBase<T>,
	internal_::DummyStreamingBase
>::type;


} // namespace metapp


#endif

