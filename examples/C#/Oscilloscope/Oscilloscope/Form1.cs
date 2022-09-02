using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using OxyPlot.WindowsForms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Oscilloscope
{
    public partial class OscilloscopeForm : Form
    {
        const int x_axis_min_value = 0;// minimum value of x axis
        const int x_axis_max_value = 100;// maximum value of x axis

        const int y_axis_min_value = 0;// minimum value of y axis
        const int y_axis_max_value = 100;// maximum value of y axis

        PlotView plot_view = null;

        public OscilloscopeForm()
        {
            InitializeComponent();
        }

        private void OscilloscopeForm_Load(object sender, EventArgs e)
        {
            plot_view = new PlotView();
            plot_view.Location = new Point(0, 0);
            plot_view.Size = new Size(1000, 500);
            this.Controls.Add(plot_view);

            LinearAxis x_axis = new LinearAxis { Position = AxisPosition.Bottom, Minimum = x_axis_min_value, Maximum = x_axis_max_value };
            LinearAxis y_axis = new LinearAxis { Position = AxisPosition.Left, Minimum = y_axis_min_value, Maximum = y_axis_max_value };

            plot_view.Model = new PlotModel { Title = "Oscilloscope" };
            Thread t = new Thread(DisplayRandomSeries);
            t.Start();
        }

        //method used to display a random series on chart
        private void DisplayRandomSeries()
        {
            Random rnd = new Random(DateTime.Now.Millisecond);
            FunctionSeries series = new FunctionSeries();
            while (true)
            {
                series.Points.Clear();
                plot_view.Model.InvalidatePlot(true);
                for (int i = 0; i < 500; i++)
                {
                    int y_pos = rnd.Next(0, 1000);
                    series.Points.Add(new DataPoint(i, y_pos));
                }

                plot_view.Model.Series.Add(series);
                Thread.Sleep(40);
                plot_view.Model.Series.Clear();
                plot_view.Model.InvalidatePlot(true);
            }
        }


    }
}
