#pragma once
#include"sign.hpp"   //basic
#include"str.hpp"   //basic
#include"index.hpp"   //based on str.hpp
#include"judge.hpp"   //base on sign.hpp
#include"path.hpp"   //based on str.hpp
#include"ncnn.hpp"   //based on str.hpp & index.hpp
#include"points.hpp"   //based on str.hpp, index.hpp & judge.hpp
#include"cv.hpp"   //based on str.hpp & points.hpp
#include"print.hpp"   //based on judge.hpp & sign.hpp, ands expand by points.hpp & cv.hpp

/*
TOP					   print.hpp
						cv.hpp
				 ncnn.hpp    points.hpp    
		  index.hpp    judge.hpp    path.hpp
BASE		      sign.hpp    str.hpp
*/