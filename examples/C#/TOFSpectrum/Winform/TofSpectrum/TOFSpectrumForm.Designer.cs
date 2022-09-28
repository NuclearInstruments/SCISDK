namespace Oscilloscope
{
    partial class TOFSpectrumForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TOFSpectrumForm));
            this.btn_start = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.btn_reset_zoom = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cmb_refresh_time = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // btn_start
            // 
            this.btn_start.BackColor = System.Drawing.Color.Transparent;
            this.btn_start.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("btn_start.BackgroundImage")));
            this.btn_start.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.btn_start.Location = new System.Drawing.Point(12, 12);
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
            this.btn_stop.Location = new System.Drawing.Point(53, 12);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(35, 35);
            this.btn_stop.TabIndex = 2;
            this.btn_stop.UseVisualStyleBackColor = false;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // btn_reset_zoom
            // 
            this.btn_reset_zoom.Location = new System.Drawing.Point(94, 18);
            this.btn_reset_zoom.Name = "btn_reset_zoom";
            this.btn_reset_zoom.Size = new System.Drawing.Size(75, 23);
            this.btn_reset_zoom.TabIndex = 22;
            this.btn_reset_zoom.Text = "Reset zoom";
            this.btn_reset_zoom.UseVisualStyleBackColor = true;
            this.btn_reset_zoom.Click += new System.EventHandler(this.btn_reset_zoom_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(837, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 13);
            this.label1.TabIndex = 24;
            this.label1.Text = "Refresh";
            // 
            // cmb_refresh_time
            // 
            this.cmb_refresh_time.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cmb_refresh_time.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmb_refresh_time.FormattingEnabled = true;
            this.cmb_refresh_time.Location = new System.Drawing.Point(886, 15);
            this.cmb_refresh_time.Name = "cmb_refresh_time";
            this.cmb_refresh_time.Size = new System.Drawing.Size(121, 21);
            this.cmb_refresh_time.TabIndex = 25;
            this.cmb_refresh_time.SelectedIndexChanged += new System.EventHandler(this.cmb_refresh_time_SelectedIndexChanged);
            // 
            // TOFSpectrumForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1019, 575);
            this.Controls.Add(this.cmb_refresh_time);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btn_reset_zoom);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.btn_start);
            this.Name = "TOFSpectrumForm";
            this.Text = "Oscilloscope";
            this.Load += new System.EventHandler(this.OscilloscopeForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.Button btn_reset_zoom;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmb_refresh_time;
    }
}

