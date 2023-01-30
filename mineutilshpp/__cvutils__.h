#pragma once
#include"sign.hpp"   //basic
#include"litetime.hpp"   //basic
#include"str.hpp"   //basic
#include"exception.hpp"   //based on str.hpp
#include"judge.hpp"   //base on sign.hpp
#include"file.hpp"   //based on str.hpp & exception.hpp
#include"index.hpp"   //based on str.hpp & exception.hpp
#include"path.hpp"   //based on str.hpp & exception.hpp
#include"points.hpp"   //based on str.hpp, exception.hpp, judge.hpp & index.hpp
#include"cv.hpp"   //based on str.hpp & points.hpp
#include"print.hpp"   //based on judge.hpp & sign.hpp, ands expand by points.hpp & cv.hpp

/*
TOP					 print.hpp
					  cv.hpp
			  (ncnn.hpp)    points.hpp
			file.hpp    index.hpp    path.hpp
			 exception.hpp    judge.hpp
BASE	 sign.hpp    litetime.hpp    str.hpp
*/