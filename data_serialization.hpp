#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class s_buffer
{
public:
	std::shared_ptr<char>  m_t;
	uint16_t m_size;

	void* getbuffer()
	{
		return static_cast<void*>(m_t.get());
	}
	uint16_t getsize()
	{
		return m_size;
	}
	template<class Archive>
	void save(Archive & ar, const unsigned int /* file_version */) const {
		ar & m_size;
		ar.save_binary(static_cast<void*>(m_t.get()), m_size);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int /* file_version */)  {
		ar & m_size;
		m_t.reset(new char[m_size]);
		ar.load_binary(static_cast<void *>(m_t.get()), m_size);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

		s_buffer() :m_t(nullptr), m_size(0)
	{}
	s_buffer(char* buffer, uint16_t size ) :m_t(buffer), m_size(size)
	{}
};


template <class T>
std::shared_ptr<s_buffer> get_s_buffer(T& data)
{
	std::stringstream stream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
	boost::archive::binary_oarchive oa(stream, 15);
	oa & data;

	stream.seekp(0, std::ios::end);
	std::size_t size = stream.tellp();
	stream.seekp(0, std::ios::beg);

	char* buffer = new char[size];
	stream.read(buffer, size);
	return std::shared_ptr<s_buffer>(new s_buffer(buffer, size));
}

template <class T, class T2>
void get_data(T& data, T2& buffer)
{
	std::stringstream stream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
	stream.write(static_cast<char*>(buffer.getbuffer()), buffer.getsize());
	boost::archive::binary_iarchive ia(stream, 15);
	ia & data;
}

template <class T, class T2>
void _data_serialize(bool _save, T& _serializedata, int  index, T2& value)
{
	if (_save)
	{
		_serializedata[index] = *get_s_buffer(value);
	}
	else
	{
		auto ifind = _serializedata.find(index);
		if (ifind != _serializedata.end())
		{
			get_data(value, ifind->second);
		}
	}
}

template<class T>
class base_serialize
{
public:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version )
	{
		if (Archive::is_saving::value)
		{
			std::map < uint8_t, s_buffer > tempdata;
			This()->data_serialize(true, tempdata, file_version);
			ar & tempdata;
		}
		else
		{
			std::map <uint8_t, s_buffer > tempdata;
			ar & tempdata;
			This()->data_serialize(false, tempdata, file_version);
		}
	}

	T* This()
	{
		return static_cast<T*> (this);
	}
};
