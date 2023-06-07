namespace Refprop_Deneme
{
    partial class Form1
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
            this.calculate = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.property_1_value = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.property_2_value = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.output_property_value = new System.Windows.Forms.TextBox();
            this.property_1 = new System.Windows.Forms.ComboBox();
            this.property_2 = new System.Windows.Forms.ComboBox();
            this.output_property = new System.Windows.Forms.ComboBox();
            this.refrigerant = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.property_1_unit = new System.Windows.Forms.Label();
            this.property_2_unit = new System.Windows.Forms.Label();
            this.output_unit = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.phase_textbox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.dll_version_label = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // calculate
            // 
            this.calculate.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.calculate.Location = new System.Drawing.Point(20, 342);
            this.calculate.Name = "calculate";
            this.calculate.Size = new System.Drawing.Size(86, 32);
            this.calculate.TabIndex = 0;
            this.calculate.Text = "Calculate";
            this.calculate.UseVisualStyleBackColor = true;
            this.calculate.Click += new System.EventHandler(this.calculate_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(16, 60);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 20);
            this.label1.TabIndex = 2;
            this.label1.Text = "Property-1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(16, 216);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(121, 20);
            this.label2.TabIndex = 4;
            this.label2.Text = "Output Property";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(16, 138);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(82, 20);
            this.label3.TabIndex = 6;
            this.label3.Text = "Property-2";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(16, 99);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(127, 20);
            this.label4.TabIndex = 8;
            this.label4.Text = "Property-1 Value";
            // 
            // property_1_value
            // 
            this.property_1_value.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.property_1_value.Location = new System.Drawing.Point(214, 102);
            this.property_1_value.Name = "property_1_value";
            this.property_1_value.Size = new System.Drawing.Size(261, 26);
            this.property_1_value.TabIndex = 7;
            this.property_1_value.Text = "27";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(16, 177);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(127, 20);
            this.label5.TabIndex = 10;
            this.label5.Text = "Property-2 Value";
            // 
            // property_2_value
            // 
            this.property_2_value.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.property_2_value.Location = new System.Drawing.Point(214, 178);
            this.property_2_value.Name = "property_2_value";
            this.property_2_value.Size = new System.Drawing.Size(261, 26);
            this.property_2_value.TabIndex = 9;
            this.property_2_value.Text = "1";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(16, 272);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(166, 20);
            this.label6.TabIndex = 12;
            this.label6.Text = "Output Property Value";
            // 
            // output_property_value
            // 
            this.output_property_value.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.output_property_value.Location = new System.Drawing.Point(214, 266);
            this.output_property_value.Name = "output_property_value";
            this.output_property_value.Size = new System.Drawing.Size(261, 26);
            this.output_property_value.TabIndex = 11;
            // 
            // property_1
            // 
            this.property_1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.property_1.FormattingEnabled = true;
            this.property_1.Items.AddRange(new object[] {
            "T",
            "P",
            "H",
            "S",
            "D",
            "Q"});
            this.property_1.Location = new System.Drawing.Point(214, 63);
            this.property_1.Name = "property_1";
            this.property_1.Size = new System.Drawing.Size(261, 28);
            this.property_1.TabIndex = 13;
            this.property_1.Text = "T";
            this.property_1.SelectedIndexChanged += new System.EventHandler(this.property_1_SelectedIndexChanged);
            // 
            // property_2
            // 
            this.property_2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.property_2.FormattingEnabled = true;
            this.property_2.Items.AddRange(new object[] {
            "T",
            "P",
            "H",
            "S",
            "D",
            "Q"});
            this.property_2.Location = new System.Drawing.Point(214, 139);
            this.property_2.Name = "property_2";
            this.property_2.Size = new System.Drawing.Size(261, 28);
            this.property_2.TabIndex = 14;
            this.property_2.Text = "Q";
            this.property_2.SelectedIndexChanged += new System.EventHandler(this.property_2_SelectedIndexChanged);
            // 
            // output_property
            // 
            this.output_property.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.output_property.FormattingEnabled = true;
            this.output_property.Items.AddRange(new object[] {
            "T",
            "P",
            "H",
            "S",
            "D",
            "Q"});
            this.output_property.Location = new System.Drawing.Point(214, 215);
            this.output_property.Name = "output_property";
            this.output_property.Size = new System.Drawing.Size(261, 28);
            this.output_property.TabIndex = 15;
            this.output_property.Text = "P";
            this.output_property.SelectedIndexChanged += new System.EventHandler(this.output_property_SelectedIndexChanged);
            // 
            // refrigerant
            // 
            this.refrigerant.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.refrigerant.FormattingEnabled = true;
            this.refrigerant.Items.AddRange(new object[] {
            "R134A",
            "R407C",
            "R227EA",
            "R236FA",
            "R450A",
            "R22",
            "R32",
            "R410A",
            "R134a;0.44; R1234yf;0.56;",
            "R227ea;0.089;r1234zee;0.911",
            "WATER",
            "R515A"});
            this.refrigerant.Location = new System.Drawing.Point(214, 24);
            this.refrigerant.Name = "refrigerant";
            this.refrigerant.Size = new System.Drawing.Size(261, 28);
            this.refrigerant.TabIndex = 17;
            this.refrigerant.Text = "R134A";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(16, 24);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 20);
            this.label7.TabIndex = 16;
            this.label7.Text = "Refrigerant";
            // 
            // property_1_unit
            // 
            this.property_1_unit.AutoSize = true;
            this.property_1_unit.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.property_1_unit.Location = new System.Drawing.Point(480, 108);
            this.property_1_unit.Name = "property_1_unit";
            this.property_1_unit.Size = new System.Drawing.Size(25, 20);
            this.property_1_unit.TabIndex = 19;
            this.property_1_unit.Text = "°C";
            // 
            // property_2_unit
            // 
            this.property_2_unit.AutoSize = true;
            this.property_2_unit.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.property_2_unit.Location = new System.Drawing.Point(480, 181);
            this.property_2_unit.Name = "property_2_unit";
            this.property_2_unit.Size = new System.Drawing.Size(101, 20);
            this.property_2_unit.TabIndex = 20;
            this.property_2_unit.Text = "kg-vap/kg-tot";
            // 
            // output_unit
            // 
            this.output_unit.AutoSize = true;
            this.output_unit.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.output_unit.Location = new System.Drawing.Point(480, 269);
            this.output_unit.Name = "output_unit";
            this.output_unit.Size = new System.Drawing.Size(43, 20);
            this.output_unit.TabIndex = 21;
            this.output_unit.Text = "Bara";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(16, 310);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(54, 20);
            this.label10.TabIndex = 23;
            this.label10.Text = "Phase";
            // 
            // phase_textbox
            // 
            this.phase_textbox.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.phase_textbox.Location = new System.Drawing.Point(214, 304);
            this.phase_textbox.Name = "phase_textbox";
            this.phase_textbox.Size = new System.Drawing.Size(261, 26);
            this.phase_textbox.TabIndex = 22;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(210, 348);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(109, 20);
            this.label9.TabIndex = 24;
            this.label9.Text = "DLL Verison : ";
            // 
            // dll_version_label
            // 
            this.dll_version_label.AutoSize = true;
            this.dll_version_label.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dll_version_label.Location = new System.Drawing.Point(336, 348);
            this.dll_version_label.Name = "dll_version_label";
            this.dll_version_label.Size = new System.Drawing.Size(0, 20);
            this.dll_version_label.TabIndex = 25;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(596, 398);
            this.Controls.Add(this.dll_version_label);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.phase_textbox);
            this.Controls.Add(this.output_unit);
            this.Controls.Add(this.property_2_unit);
            this.Controls.Add(this.property_1_unit);
            this.Controls.Add(this.refrigerant);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.output_property);
            this.Controls.Add(this.property_2);
            this.Controls.Add(this.property_1);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.output_property_value);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.property_2_value);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.property_1_value);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.calculate);
            this.Name = "Form1";
            this.Text = "Refprop_CSharp";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button calculate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox property_1_value;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox property_2_value;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox output_property_value;
        private System.Windows.Forms.ComboBox property_1;
        private System.Windows.Forms.ComboBox property_2;
        private System.Windows.Forms.ComboBox output_property;
        private System.Windows.Forms.ComboBox refrigerant;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label property_1_unit;
        private System.Windows.Forms.Label property_2_unit;
        private System.Windows.Forms.Label output_unit;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox phase_textbox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label dll_version_label;
    }
}

