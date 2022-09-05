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
        PlotView plot_view = null;

        IntPtr _scisdk_handle;// pointer to the scisdk handle
        string _board_name;// name given from user to the board
        string _json_board_path;// path to json project file

        public OscilloscopeForm(IntPtr scisdk_handle, string board_name, string json_file_path)
        {
            InitializeComponent();
            
            _scisdk_handle = scisdk_handle;
            _board_name = board_name;



            cmb_data_type.SelectedIndex = 0;
            check_lst_channels.Items.Add("ALL");
        }

        private void OscilloscopeForm_Load(object sender, EventArgs e)
        {
            // create and display wave graph container
            plot_view = new PlotView();
            plot_view.Location = new Point(250, 65);
            plot_view.Size = new Size(750, 500);
            this.Controls.Add(plot_view);
            LinearAxis x_axis = new LinearAxis { Position = AxisPosition.Bottom, Title = "Time(ns)" };
            LinearAxis y_axis = new LinearAxis { Position = AxisPosition.Left, Title = "" };
            plot_view.Model = new PlotModel();
            plot_view.Model.Axes.Add(x_axis);
            plot_view.Model.Axes.Add(y_axis);

            Thread t = new Thread(DisplayRandomSeries);
            //t.Start();
        }

        //method used to display a random series on chart
        private void DisplayRandomSeries()
        {

            Random rnd = new Random(DateTime.Now.Millisecond);

            while (true)
            {
                FunctionSeries series = new FunctionSeries();
                series.Points.Clear();
                plot_view.Model.InvalidatePlot(true);
                for (int i = 0; i < 500; i++)
                {
                    int y_pos = rnd.Next(0, 1000);
                    series.Points.Add(new DataPoint(i, y_pos));
                }

                plot_view.Model.Series.Add(series);
                Thread.Sleep(25);
                plot_view.Model.Series.Clear();
                plot_view.Model.InvalidatePlot(true);

            }
        }
    }


}

