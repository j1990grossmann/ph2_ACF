/*
 * File:   MuxTest.h
 * Author: Johannes Grossmann
 * Distributed under the Boost Software License, Version 1.0.
 * Created on February 19th, 2015, 10:46 AM
 */

#ifndef RUNCOUNTER_H
#define	RUNCOUNTER_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <string.h>
#include <vector>

#include <boost/filesystem.hpp>

using namespace std;

namespace RUNCOUNTER{
	
	class Runcounter
	{
	public:
		Runcounter(){
			
		}
		~Runcounter() {
			
		}
		// methods
		string GetFileName();
		void ResetCount();
	private:
		int counter;		
	protected:
	};
};

#endif 
