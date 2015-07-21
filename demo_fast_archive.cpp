/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// demo_fast_binary_archive.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution
#include <sstream>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/serialization/pfto.hpp>

#define BOOST_ARCHIVE_SOURCE

#include <boost/archive/detail/register_archive.hpp>

// include template definitions for base classes used.  Otherwise
// you'll get link failure with undefined symbols

#include <boost/archive/impl/basic_binary_oarchive.ipp>
#include <boost/archive/impl/basic_binary_iarchive.ipp>

#include "data_serialization.hpp"




#include <iostream>

using namespace boost::archive;



#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <iostream>
#include <fstream>
#include <string>
  #include <boost/date_time/posix_time/posix_time.hpp>  
template<class T>
void save_schedule(const T &s, const char * filename){
	// make an archive
	std::ofstream ofs(filename, std::ios_base::binary);
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();  
	boost::archive::binary_oarchive oa(ofs);
	//boost::archive::text_oarchive oa(ofs);


	oa << s;

	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();  
	std::string strTime = boost::posix_time::to_iso_string(ptEnd-ptStart);  
	std::cout << "save time: " << strTime << std::endl;  
}

template<class T>
void restore_schedule(T &s, const char * filename)
{
	// open the archive
	std::ifstream ifs(filename, std::ios_base::binary);
	boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();  
	boost::archive::binary_iarchive ia(ifs);
	ia >> s;
	boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();  
	std::string strTime = boost::posix_time::to_iso_string(ptEnd-ptStart);  
	std::cout << "save time: " << strTime << std::endl;  
}

class gps_position :public base_serialize<gps_position>
{
	int degrees;
	int minutes;
	float seconds;
public:
	template <class T>
	void data_serialize(bool _save, T& _serializedata, const unsigned int file_version)
	{
		_data_serialize(_save, _serializedata, 1, this->degrees);
		_data_serialize(_save, _serializedata, 2, this->minutes);
		_data_serialize(_save, _serializedata, 3, this->seconds);

		//std::cout << "version :" << file_version << std::endl;
	}
	// every serializable class needs a constructor
	gps_position(){
		 degrees = 0;
		 minutes = 0;
		 seconds = 0;
	};
	gps_position(int _d, int _m, float _s) :
		degrees(_d), minutes(_m), seconds(_s)
	{}
	void print()
	{
		std::cout << degrees << " " << minutes << " " << seconds << std::endl;
	}
};
BOOST_CLASS_VERSION(gps_position, 2)


int main(int argc, char* argv[])
{
	//const int a[3] = { 1, 2, 3 };
	//int a1[3] = { 4, 5, 6 };

	std::map<int, gps_position> a;
	std::map<int, gps_position> a1;

	a[1] = gps_position(1, 2, 3);
	a[2] = gps_position(34, 2, 3);

	a[3] = gps_position(14, 2, 3);

	a[4] = gps_position(134, 2, 3);

	a[6] = gps_position(21, 2, 3);
	a[7] = gps_position(18, 2, 3);

	a[8] = gps_position(19, 2, 3);

	a[9] = gps_position(19, 2, 3);
	for (int i = 100; i < 1000000; i++)
	{
		a[i] = gps_position(19, 2, 3);
	}

	std::string filename = "demofile.txt";

	// save the schedule
	save_schedule(a, filename.c_str());

	restore_schedule(a1, filename.c_str());

	//std::cout << a1[1] << a1[2];
	//std::for_each(a1.begin(), a1.end(), [](std::pair<int, gps_position> n) { n.second.print(); });

	//system("pause");
	//return (a[0] != a1[0]) || (a[1] != a1[1]) || (a[2] != a1[2]);
	return 0;
}


