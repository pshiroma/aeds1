#include "xplot/xfigure.hpp"
#include "xplot/xmarks.hpp"
#include "xplot/xaxes.hpp"
#include <random>
#include <vector>
#include <array>
#include <map>
#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end
#include <iostream>

namespace aeds1
{
	#define MAXFIG 20 // define o numero maximo de figuras disponivel em um notebook
	
	// estrutura usada para armazenar um curva: pontos x,y e o objeto de desenho
	typedef struct {
		xpl::lines line;
		std::vector<double> x;
		std::vector<double> y;
	} T_CURVA;
	
	std::array<std::map<int, T_CURVA>, MAXFIG> curvas; // todas as curvas de todas as figuras

	std::array<xpl::figure, MAXFIG> figuras; // cria um vetor com MAXFIG figuras
	std::array<xpl::lines, MAXFIG> eixos_x, eixos_y; // os eixos x e y de cada figura
	std::array<xpl::scatter, MAXFIG> extremos_x, extremos_y;
	
	std::array<xpl::linear_scale, MAXFIG> sx, sy;
	
	void inicializa() {
		for (int i = 0; i < MAXFIG; i++) {
			// criando os eixos x, y e configurando-os
			eixos_x[i] = xpl::lines(sx[i],sy[i]);
			eixos_y[i] = xpl::lines(sx[i],sy[i]);

			extremos_x[i] = xpl::scatter::initialize(aeds1::sx[i], aeds1::sy[i]) .finalize();
			extremos_y[i] = xpl::scatter::initialize(aeds1::sx[i], aeds1::sy[i]) .finalize();
			
			eixos_x[i].opacities = std::vector<double>({0.3});
			eixos_x[i].colors = std::vector<std::string>({"red"});
			eixos_x[i].line_style = std::string("dashed");

			eixos_y[i].opacities = std::vector<double>({0.3});
			eixos_y[i].colors = std::vector<std::string>({"red"});
			eixos_y[i].line_style = std::string("dashed");

			eixos_x[i].x = std::vector<double>({0,1});
			eixos_x[i].y = std::vector<double>({0,0});
			eixos_y[i].x = std::vector<double>({0,0});
			eixos_y[i].y = std::vector<double>({0,1});

			extremos_x[i].x = std::vector<double>({0,1});
			extremos_x[i].y = std::vector<double>({0,0});
			extremos_y[i].x = std::vector<double>({0,0});
			extremos_y[i].y = std::vector<double>({0,1});
			
			// adicionando os eixos à figura
			figuras[i].add_mark(eixos_x[i]);
			figuras[i].add_mark(eixos_y[i]);
			
			figuras[i].add_mark(extremos_x[i]);
			figuras[i].add_mark(extremos_y[i]);
		}
	}

	// retorna a cor padrão a ser usada em cada curva, dentro de uma figura
	std::vector<std::string> getColor(int idx) {
		switch (idx) {
			case 0: return std::vector<std::string>({"blue"});
			case 1: return std::vector<std::string>({"red"});
			case 2: return std::vector<std::string>({"purple"});
			case 3: return std::vector<std::string>({"black"});
			default : return std::vector<std::string>({"magenta"});
		}
	}

	// atualiza os eixos x e y para o intervalo de dados
	void atualizaEixos(int figura = 0) {
		double xmin, xmax, ymin, ymax;
		bool primeiro_x = true, primeiro_y = true;
		for (auto const& p : aeds1::curvas[figura]) {
			for ( auto const& valor : p.second.x) {
				if (primeiro_x) {
					primeiro_x = false;
					xmin = xmax = valor;
				}
				if (valor < xmin)
					xmin = valor;
				if (valor > xmax)
					xmax = valor;
			}
			for ( auto const& valor : p.second.y) {
				if (primeiro_y) {
					primeiro_y = false;
					ymin = ymax = valor;
				}
				if (valor < ymin)
					ymin = valor;
				if (valor > ymax)
					ymax = valor;
			}
		}
		eixos_x[figura].x = std::vector<double>({xmin,xmax});
		//eixos_x[figura].y = std::vector<double>({ymin,ymin});
		//eixos_y[figura].x = std::vector<double>({xmin,xmin});
		eixos_y[figura].y = std::vector<double>({ymin,ymax});
		
		extremos_x[figura].x = std::vector<double>({xmin,xmax});
		extremos_x[figura].names = std::vector<std::string>{std::to_string(xmin), std::to_string(xmax)};

		extremos_y[figura].y = std::vector<double>({ymin,ymax});
		extremos_y[figura].names = std::vector<std::string>{std::to_string(ymin), std::to_string(ymax)};
		//std::cout << xmin << " " << xmax << " " << ymin << " " << ymax << std::endl;
	}

	// apaga uma curva de uma figura
	void plot_apaga(int _curva = 0, int figura = 0)
	{
		if ( curvas[figura].count( _curva ) ) { // verifica se a curva existe
			curvas[figura][ _curva ].x.clear();
			curvas[figura][ _curva ].y.clear();
			curvas[figura][ _curva ].line.x = curvas[figura][ _curva ].x;
			curvas[figura][ _curva ].line.y = curvas[figura][ _curva ].y;
			atualizaEixos(figura);
		}
		
	}

	// adiciona um ponto novo em uma curva de uma figura
	void plot_novo_ponto(double newx, double newy, int _curva=0, int figura = 0) {
		if (curvas[figura].count( _curva )) {
			curvas[figura][_curva].x.push_back( newx);
			curvas[figura][_curva].y.push_back( newy);
			curvas[figura][_curva].line.x = curvas[figura][_curva].x;
			curvas[figura][_curva].line.y = curvas[figura][_curva].y;
		} else { // cria uma nova curva
			T_CURVA tmp;
			tmp.line = xpl::lines(sx[figura],sy[figura]);
			tmp.line.colors = getColor(_curva);
			curvas[figura].insert({_curva, tmp});
			figuras[figura].add_mark(curvas[figura][_curva].line);
		}
		atualizaEixos(figura);
	}
	
	// apaga um ponto de uma curva de uma figura
	void plot_apaga_ponto(int _curva=0, int figura = 0)
	{
		if ( curvas[figura].count( _curva ) ) {
			curvas[figura][_curva].x.pop_back();
			curvas[figura][_curva].y.pop_back();
			curvas[figura][_curva].line.x = curvas[figura][_curva].x;
			curvas[figura][_curva].line.y = curvas[figura][_curva].y;
			atualizaEixos(figura);
		}
	}
	
	// exemplo
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

// inicializacao, chamada quando executamos .X aeds1_plot no jupyter
void aeds1_plot()
{
	aeds1::inicializa();
}
