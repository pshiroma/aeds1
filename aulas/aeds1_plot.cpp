#include "xplot/xfigure.hpp"
#include "xplot/xmarks.hpp"
#include "xplot/xaxes.hpp"
#include <random>
#include <vector>

namespace aeds1
{
	xpl::figure fig;
	xpl::linear_scale sx, sy;
	xpl::lines line(sx,sy);
	xpl::axis hx(sx), hy(sy);
	
	std::vector<double> x;
	std::vector<double> y;
	

	void inicializa()
	{
		line.x = x;
		line.y = y;
		hy.orientation = "vertical";
		fig.add_mark(line);
		fig.add_axis(hx);
		fig.add_axis(hy);
	}
	
	void plot_exemplo()
	{
		line.x = std::vector<double>({0.0, 0.5, 1.0, 1.1, 1.2, 1.3});
		line.y = std::vector<double>({0.0, 0.7, 0.5, 1.1, 1.2, 1.2});
	}
	void plot_apaga()
	{
		x.clear();
		y.clear();
		line.x = x;
		line.y = y;
	}
	void plot_novo_ponto(double newx, double newy)
	{
		x.push_back(newx);
		y.push_back(newy);
		line.x = x;
		line.y = y;
	}
}

void aeds1_plot()
{
	aeds1::inicializa();
}
