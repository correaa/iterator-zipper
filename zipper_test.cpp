#ifdef compile_instructions
clang++ -std=c++14 -O3 -Wfatal-errors -lboost_system -lboost_timer $0 -o $0.x -lstdc++fs && $0.x $@ && rm -rf $0.x; exit
#endif

#include "./zipper.hpp"
#include <vector>
#include <iostream>
#include <random>
#include <boost/timer/timer.hpp>
#include <algorithm> // std::sort
#include <list>

int main(){

    std::mt19937 gen;
    std::uniform_real_distribution<> dis(0, 1);

	std::clog << "define elements" << std::endl;
	int N = 10000000;
	std::vector<double> v(N);

	// generate a few random vectors
	generate(begin(v), end(v), [&dis, &gen](){return dis(gen);});
	std::vector<double> w(N);
	generate(begin(w), end(w), [&dis, &gen](){return dis(gen);});
	std::vector<double> x(N);
	generate(begin(x), end(x), [&dis, &gen](){return dis(gen);});

	using boost::zip;
	{
		std::vector<double> vv = v;
 		boost::timer::auto_cpu_timer t(std::cerr, "vector<double> sort %w seconds\n");
		std::sort(begin(vv), end(vv));
	}
	{
		std::vector<std::tuple<double, double>> vw(N);
		std::copy(zip(begin(v), begin(w)), zip(end(v), end(w)), begin(vw));
		{
	 		boost::timer::auto_cpu_timer t(std::cerr, "vector<tuple<double, double>> tuple sort %w seconds\n");
			std::sort(begin(vw), end(vw));
		}
	}
	{
		std::vector<std::tuple<double, double>> vw(N);
		std::copy(
			zip(begin(v), begin(w)), 
			zip(end(v), end(w)), 
			begin(vw)
		);
		{
	 		boost::timer::auto_cpu_timer t(std::cerr, "vector<tuple<double, double>> optimized sort %w seconds\n");
			using std::get;
			std::sort(begin(vw), end(vw), [](auto&& a, auto&& b){return get<0>(a) < get<0>(b);});
		}
	}
	{
		std::vector<double> vv = v;
		std::vector<double> ww = w;
 		boost::timer::auto_cpu_timer t(std::cerr, "zipper pair sort %w seconds\n");
		std::sort(
			zip(begin(vv), begin(ww)),
			zip(end(vv)  , end(ww)  )
		);
	}
	{
		std::vector<double> vv = v;
		std::vector<double> ww = w;
 		boost::timer::auto_cpu_timer t(std::cerr, "zipper optimized pair sort %w seconds\n");
		using std::get;
		std::sort(
			zip(vv.begin(), ww.begin()), 
			zip(vv.end()  , ww.end()  ), 
			[](auto&& a, auto&& b){return get<0>(a) < get<0>(b);}
		);
	}
	{
		std::vector<double> vv = v;
		std::vector<double> ww = w;
		std::vector<double> xx = x;

 		boost::timer::auto_cpu_timer t(std::cerr, "zipper trio sort %w seconds\n");
		std::sort(
			zip(begin(vv), zip(begin(ww), begin(xx))), 
			zip(end(vv)  , zip(end(ww)  , end(xx)  ))
		);
	}
	{
		std::vector<double> vv = v;
		std::vector<double> ww = w;
		std::vector<double> xx = x;
 		boost::timer::auto_cpu_timer t(std::cerr, "zipper optimized trio sort %w seconds\n");
		using std::get;
		std::sort(
			zip(begin(vv), begin(ww), begin(xx)), 
			zip(end(vv)  , end(ww)  , end(xx)  ), 
			[](auto&& a, auto&& b){return get<0>(a) < get<0>(b);}
		);
	}
	{
		std::list<double> l = {1.,2.,3.};
		std::vector<double> v = {1.,2.,3.};
		auto beg = zip(l.begin(), v.begin());
		auto end = zip(l.end()  , v.end()  );
	}
}

