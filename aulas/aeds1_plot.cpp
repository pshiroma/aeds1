#include "xplot/xfigure.hpp"
#include "xplot/xmarks.hpp"
#include "xplot/xaxes.hpp"
#include <random>
#include <vector>
#include <map>
#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end
//#include <iostream>

namespace aeds1
{
	xpl::figure fig;
	xpl::linear_scale sx, sy;	
	xpl::lines eixo_x(sx,sy), eixo_y(sx,sy);
	
	typedef struct {
		xpl::lines line;
		std::vector<double> x;
		std::vector<double> y;
	} T_CURVA;
	
	std::map<int, T_CURVA> curvas;
		
	std::vector<std::string> getColor(int idx) {
		switch (idx) {
			std::vector<std::string>({"red"});
			case 0: return std::vector<std::string>({"blue"});
			case 1: return std::vector<std::string>({"red"});
			case 2: return std::vector<std::string>({"purple"});
			case 3: return std::vector<std::string>({"black"});
			default : return std::vector<std::string>({"magenta"});
		}
	}

	void inicializa()
	{
		eixo_x.opacities = std::vector<double>({0.3});
		eixo_x.colors = std::vector<std::string>({"red"});
		eixo_x.line_style = std::string("dashed");

		eixo_y.opacities = std::vector<double>({0.3});
		eixo_y.colors = std::vector<std::string>({"red"});
		eixo_y.line_style = std::string("dashed");

		eixo_x.x = std::vector<double>({0,1});
		eixo_x.y = std::vector<double>({0,0});
		eixo_y.x = std::vector<double>({0,0});
		eixo_y.y = std::vector<double>({0,1});
		
		fig.add_mark(eixo_x);
		fig.add_mark(eixo_y);
	}

	void atualizaEixos() {
		double xmin, xmax, ymin, ymax;
		xmin = xmax = ymin = ymax = 0.0;
		for (auto const& p : aeds1::curvas) {
			for ( auto const& valor : p.second.x) {
				if (valor < xmin)
					xmin = valor;
				if (valor > xmax)
					xmax = valor;
			}
			for ( auto const& valor : p.second.y) {
				if (valor < ymin)
					ymin = valor;
				if (valor > ymax)
					ymax = valor;
			}
		}
		eixo_x.x = std::vector<double>({xmin,xmax});
		eixo_y.y = std::vector<double>({ymin,ymax});
		//std::cout << xmin << " " << xmax << " " << ymin << " " << ymax << std::endl;
	}

	void plot_apaga(int _curva = 0)
	{
		if ( curvas.count( _curva ) ) {
			curvas[ _curva ].x.clear();
			curvas[ _curva ].y.clear();
			curvas[ _curva ].line.x = curvas[ _curva ].x;
			curvas[ _curva ].line.y = curvas[ _curva ].y;
		}
		atualizaEixos();
	}

	void plot_novo_ponto(double newx, double newy, int _curva=0) {
		if (curvas.count( _curva )) {
			curvas[_curva].x.push_back( newx);
			curvas[_curva].y.push_back( newy);
			curvas[_curva].line.x = curvas[_curva].x;
			curvas[_curva].line.y = curvas[_curva].y;
		} else { // cria uma nova curva
			T_CURVA tmp;
			tmp.line = xpl::lines(sx,sy);
			tmp.line.colors = getColor(_curva);
			curvas.insert({_curva, tmp});
			fig.add_mark(curvas[_curva].line);
		}
		atualizaEixos();
	}
	
	
	void plot_apaga_ponto(int _curva=0)
	{
		if ( curvas.count( _curva ) ) {
			curvas[_curva].x.pop_back();
			curvas[_curva].y.pop_back();
			curvas[_curva].line.x = curvas[_curva].x;
			curvas[_curva].line.y = curvas[_curva].y;
			atualizaEixos();
		}
	}
			
	void plot_exemplo()
	{		
		plot_novo_ponto(0,0);
		plot_novo_ponto(1,1);
		plot_novo_ponto(2,1);
		plot_novo_ponto(3,0.5);
		plot_novo_ponto(4,0.3);
		plot_novo_ponto(5,0.3);
	}
}

void aeds1_plot()
{
	aeds1::inicializa();
}
