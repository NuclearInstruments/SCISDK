namespace Oscilloscope
{
    partial class OscilloscopeForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OscilloscopeForm));
            this.check_lst_channels = new System.Windows.Forms.CheckedListBox();
            this.btn_start = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.lbl2 = new System.Windows.Forms.Label();
            this.lbl_status = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txt_horizontal_divisions = new System.Windows.Forms.TextBox();
            this.track_horizontal_divisions = new System.Windows.Forms.TrackBar();
            this.track_pretrigger = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.txt_pretrigger = new System.Windows.Forms.TextBox();
            this.track_trigger_level = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.txt_trigger_level = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.cmb_trigger_mode = new System.Windows.Forms.ComboBox();
            this.cmb_trigger_source = new System.Windows.Forms.ComboBox();
            this.cmb_trigger_edge = new System.Windows.Forms.ComboBox();
            ((System.ComponentModel.ISupportInitialize)(this.track_horizontal_divisions)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.track_pretrigger)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.track_trigger_level)).BeginInit();
            this.SuspendLayout();
            // 
            // check_lst_channels
            // 
            this.check_lst_channels.FormattingEnabled = true;
            this.check_lst_channels.Location = new System.Drawing.Point(12, 12);
            this.check_lst_channels.Name = "check_lst_channels";
            this.check_lst_channels.Size = new System.Drawing.Size(216, 214);
            this.check_lst_channels.TabIndex = 0;
            this.check_lst_channels.MouseUp += new System.Windows.Forms.MouseEventHandler(this.check_lst_channels_MouseUp);
            // 
            // btn_start
            // 
            this.btn_start.BackColor = System.Drawing.Color.Transparent;
            this.btn_start.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_start.BackgroundImage")));
            this.btn_start.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_start.Location = new System.Drawing.Point(248, 19);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(35, 35);
            this.btn_start.TabIndex = 1;
            this.btn_start.UseVisualStyleBackColor = false;
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // btn_stop
            // 
            this.btn_stop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.btn_stop.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_stop.BackgroundImage")));
            this.btn_stop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_stop.Location = new System.Drawing.Point(293, 19);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(35, 35);
            this.btn_stop.TabIndex = 2;
            this.btn_stop.UseVisualStyleBackColor = false;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // lbl2
            // 
            this.lbl2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lbl2.AutoSize = true;
            this.lbl2.Location = new System.Drawing.Point(795, 31);
            this.lbl2.Name = "lbl2";
            this.lbl2.Size = new System.Drawing.Size(50, 13);
            this.lbl2.TabIndex = 6;
            this.lbl2.Text = "STATUS";
            // 
            // lbl_status
            // 
            this.lbl_status.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lbl_status.AutoSize = true;
            this.lbl_status.ForeColor = System.Drawing.SystemColors.ControlDarkDark;
            this.lbl_status.Location = new System.Drawing.Point(852, 31);
            this.lbl_status.Margin = new System.Windows.Forms.Padding(0);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(31, 13);
            this.lbl_status.TabIndex = 7;
            this.lbl_status.Text = "IDLE";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 428);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(93, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Horizontal (ns/div)";
            // 
            // txt_horizontal_divisions
            // 
            this.txt_horizontal_divisions.Location = new System.Drawing.Point(111, 425);
            this.txt_horizontal_divisions.Name = "txt_horizontal_divisions";
            this.txt_horizontal_divisions.Size = new System.Drawing.Size(120, 20);
            this.txt_horizontal_divisions.TabIndex = 9;
            this.txt_horizontal_divisions.Text = "12000";
            this.txt_horizontal_divisions.TextChanged += new System.EventHandler(this.txt_horizontal_divisions_TextChanged);
            // 
            // track_horizontal_divisions
            // 
            this.track_horizontal_divisions.LargeChange = 100;
            this.track_horizontal_divisions.Location = new System.Drawing.Point(12, 448);
            this.track_horizontal_divisions.Maximum = 74999;
            this.track_horizontal_divisions.Minimum = 12000;
            this.track_horizontal_divisions.Name = "track_horizontal_divisions";
            this.track_horizontal_divisions.Size = new System.Drawing.Size(216, 45);
            this.track_horizontal_divisions.SmallChange = 100;
            this.track_horizontal_divisions.TabIndex = 0;
            this.track_horizontal_divisions.Value = 12000;
            this.track_horizontal_divisions.Scroll += new System.EventHandler(this.track_horizontal_divisions_Scroll);
            // 
            // track_pretrigger
            // 
            this.track_pretrigger.LargeChange = 100;
            this.track_pretrigger.Location = new System.Drawing.Point(12, 518);
            this.track_pretrigger.Maximum = 99;
            this.track_pretrigger.Name = "track_pretrigger";
            this.track_pretrigger.Size = new System.Drawing.Size(216, 45);
            this.track_pretrigger.SmallChange = 100;
            this.track_pretrigger.TabIndex = 10;
            this.track_pretrigger.Value = 20;
            this.track_pretrigger.Scroll += new System.EventHandler(this.track_pretrigger_Scroll);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 502);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "Pretrigger (%)";
            // 
            // txt_pretrigger
            // 
            this.txt_pretrigger.Location = new System.Drawing.Point(111, 499);
            this.txt_pretrigger.Name = "txt_pretrigger";
            this.txt_pretrigger.Size = new System.Drawing.Size(120, 20);
            this.txt_pretrigger.TabIndex = 12;
            this.txt_pretrigger.Text = "20";
            this.txt_pretrigger.MouseLeave += new System.EventHandler(this.txt_pretrigger_MouseLeave);
            // 
            // track_trigger_level
            // 
            this.track_trigger_level.LargeChange = 100;
            this.track_trigger_level.Location = new System.Drawing.Point(12, 378);
            this.track_trigger_level.Maximum = 65535;
            this.track_trigger_level.Name = "track_trigger_level";
            this.track_trigger_level.Size = new System.Drawing.Size(216, 45);
            this.track_trigger_level.SmallChange = 100;
            this.track_trigger_level.TabIndex = 13;
            this.track_trigger_level.Value = 1000;
            this.track_trigger_level.Scroll += new System.EventHandler(this.track_trigger_level_Scroll);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 362);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(87, 13);
            this.label4.TabIndex = 14;
            this.label4.Text = "Trigger level (lsb)";
            // 
            // txt_trigger_level
            // 
            this.txt_trigger_level.Location = new System.Drawing.Point(111, 359);
            this.txt_trigger_level.Name = "txt_trigger_level";
            this.txt_trigger_level.Size = new System.Drawing.Size(120, 20);
            this.txt_trigger_level.TabIndex = 15;
            this.txt_trigger_level.Text = "1000";
            this.txt_trigger_level.Leave += new System.EventHandler(this.txt_trigger_level_Leave);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 245);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(69, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Trigger mode";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(9, 286);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(75, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "Trigger source";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(17, 323);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(67, 13);
            this.label7.TabIndex = 18;
            this.label7.Text = "Trigger edge";
            // 
            // cmb_trigger_mode
            // 
            this.cmb_trigger_mode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_trigger_mode.FormattingEnabled = true;
            this.cmb_trigger_mode.Location = new System.Drawing.Point(105, 242);
            this.cmb_trigger_mode.Name = "cmb_trigger_mode";
            this.cmb_trigger_mode.Size = new System.Drawing.Size(123, 21);
            this.cmb_trigger_mode.TabIndex = 19;
            // 
            // cmb_trigger_source
            // 
            this.cmb_trigger_source.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_trigger_source.FormattingEnabled = true;
            this.cmb_trigger_source.Location = new System.Drawing.Point(105, 283);
            this.cmb_trigger_source.Name = "cmb_trigger_source";
            this.cmb_trigger_source.Size = new System.Drawing.Size(123, 21);
            this.cmb_trigger_source.TabIndex = 20;
            // 
            // cmb_trigger_edge
            // 
            this.cmb_trigger_edge.DisplayMember = "0";
            this.cmb_trigger_edge.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_trigger_edge.FormattingEnabled = true;
            this.cmb_trigger_edge.Location = new System.Drawing.Point(105, 320);
            this.cmb_trigger_edge.Name = "cmb_trigger_edge";
            this.cmb_trigger_edge.Size = new System.Drawing.Size(123, 21);
            this.cmb_trigger_edge.TabIndex = 21;
            // 
            // OscilloscopeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1019, 575);
            this.Controls.Add(this.cmb_trigger_edge);
            this.Controls.Add(this.cmb_trigger_source);
            this.Controls.Add(this.cmb_trigger_mode);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txt_trigger_level);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.track_trigger_level);
            this.Controls.Add(this.txt_pretrigger);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.track_pretrigger);
            this.Controls.Add(this.track_horizontal_divisions);
            this.Controls.Add(this.txt_horizontal_divisions);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.lbl_status);
            this.Controls.Add(this.lbl2);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.check_lst_channels);
            this.Name = "OscilloscopeForm";
            this.Text = "Oscilloscope";
            this.Load += new System.EventHandler(this.OscilloscopeForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.track_horizontal_divisions)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.track_pretrigger)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.track_trigger_level)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckedListBox check_lst_channels;
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.Label lbl2;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txt_horizontal_divisions;
        private System.Windows.Forms.TrackBar track_horizontal_divisions;
        private System.Windows.Forms.TrackBar track_pretrigger;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txt_pretrigger;
        private System.Windows.Forms.TrackBar track_trigger_level;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txt_trigger_level;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox cmb_trigger_mode;
        private System.Windows.Forms.ComboBox cmb_trigger_source;
        private System.Windows.Forms.ComboBox cmb_trigger_edge;
    }
}

