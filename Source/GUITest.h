#pragma once
#include "ServoHandler.h"
#include "ImageHandler.h"
#include <windows.h>
#include <opencv2\opencv.hpp>

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for GUITest
	/// </summary>
	ref class GUITest : public System::Windows::Forms::Form
	{
	public:
		
		GUITest(ServoHandler ^servoHandler, ImageHandler ^imageHandler){
			this->servoHandler = servoHandler;
			this->imageHandler = imageHandler;
			InitializeComponent();							// reference signal to servoHandler
		}



	public: 
	private:
		ServoHandler ^servoHandler;
		ImageHandler ^imageHandler;
		bool imageRun; 
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Button^  buttonRightCont;
	private: System::Windows::Forms::Button^  buttonUpCont;
	private: System::Windows::Forms::Button^  buttonDownCont;
	private: System::Windows::Forms::Button^  buttonLeftCont;
	private: System::Windows::Forms::Button^  turn_off_motors;
	private: System::Windows::Forms::Button^  stop;
	private: System::Windows::Forms::Button^  down;
	private: System::Windows::Forms::Button^  up;
	private: System::Windows::Forms::Button^  right;
	private: System::Windows::Forms::Button^  motors_on;
	private: System::Windows::Forms::Label^  label_y;
	private: System::Windows::Forms::Label^  label_x;
	private: System::Windows::Forms::Button^  left;
	private: System::Windows::Forms::Button^  vertical_autosearch;
	private: System::Windows::Forms::Button^  horizontal_autsearch;
	private: System::Windows::Forms::Label^  auto_search;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  track_button;
	private: System::Windows::Forms::Button^  stopTrackingButton;

	private: System::Windows::Forms::Button^  buttonSetHorizon;






	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~GUITest()
		{
			if (components)
			{
				delete components;
			}
		}


	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonSetHorizon = (gcnew System::Windows::Forms::Button());
			this->buttonRightCont = (gcnew System::Windows::Forms::Button());
			this->buttonUpCont = (gcnew System::Windows::Forms::Button());
			this->buttonDownCont = (gcnew System::Windows::Forms::Button());
			this->buttonLeftCont = (gcnew System::Windows::Forms::Button());
			this->turn_off_motors = (gcnew System::Windows::Forms::Button());
			this->stop = (gcnew System::Windows::Forms::Button());
			this->down = (gcnew System::Windows::Forms::Button());
			this->up = (gcnew System::Windows::Forms::Button());
			this->right = (gcnew System::Windows::Forms::Button());
			this->motors_on = (gcnew System::Windows::Forms::Button());
			this->label_y = (gcnew System::Windows::Forms::Label());
			this->label_x = (gcnew System::Windows::Forms::Label());
			this->left = (gcnew System::Windows::Forms::Button());
			this->stopTrackingButton = (gcnew System::Windows::Forms::Button());
			this->track_button = (gcnew System::Windows::Forms::Button());
			this->vertical_autosearch = (gcnew System::Windows::Forms::Button());
			this->horizontal_autsearch = (gcnew System::Windows::Forms::Button());
			this->auto_search = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->label6);
			this->splitContainer1->Panel1->Controls->Add(this->label5);
			this->splitContainer1->Panel1->Controls->Add(this->label4);
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->buttonSetHorizon);
			this->splitContainer1->Panel1->Controls->Add(this->buttonRightCont);
			this->splitContainer1->Panel1->Controls->Add(this->buttonUpCont);
			this->splitContainer1->Panel1->Controls->Add(this->buttonDownCont);
			this->splitContainer1->Panel1->Controls->Add(this->buttonLeftCont);
			this->splitContainer1->Panel1->Controls->Add(this->turn_off_motors);
			this->splitContainer1->Panel1->Controls->Add(this->stop);
			this->splitContainer1->Panel1->Controls->Add(this->down);
			this->splitContainer1->Panel1->Controls->Add(this->up);
			this->splitContainer1->Panel1->Controls->Add(this->right);
			this->splitContainer1->Panel1->Controls->Add(this->motors_on);
			this->splitContainer1->Panel1->Controls->Add(this->label_y);
			this->splitContainer1->Panel1->Controls->Add(this->label_x);
			this->splitContainer1->Panel1->Controls->Add(this->left);
			this->splitContainer1->Panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &GUITest::splitContainer1_Panel1_Paint);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->stopTrackingButton);
			this->splitContainer1->Panel2->Controls->Add(this->track_button);
			this->splitContainer1->Panel2->Controls->Add(this->vertical_autosearch);
			this->splitContainer1->Panel2->Controls->Add(this->horizontal_autsearch);
			this->splitContainer1->Panel2->Controls->Add(this->auto_search);
			this->splitContainer1->Size = System::Drawing::Size(906, 517);
			this->splitContainer1->SplitterDistance = 738;
			this->splitContainer1->TabIndex = 0;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(289, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(9, 520);
			this->label6->TabIndex = 47;
			this->label6->Text = L"|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n" 
				L"|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|\r\n|";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(28, 33);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(168, 20);
			this->label5->TabIndex = 46;
			this->label5->Text = L"MOTOR STEERING";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(427, 322);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 45;
			this->label4->Text = L"HEJ";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(344, 35);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(27, 13);
			this->label3->TabIndex = 44;
			this->label3->Text = L"HEJ";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 289);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(271, 39);
			this->label2->TabIndex = 43;
			this->label2->Text = L"These buttons can be pressed once and the motors will \r\nmove until the stop butto" 
				L"n is pressed or the limitations \r\nof the dome opening is reached";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 87);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(176, 26);
			this->label1->TabIndex = 42;
			this->label1->Text = L"These buttons will move the motors \r\nas long as the buttons are pressed";
			// 
			// buttonSetHorizon
			// 
			this->buttonSetHorizon->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonSetHorizon->Location = System::Drawing::Point(446, 424);
			this->buttonSetHorizon->Name = L"buttonSetHorizon";
			this->buttonSetHorizon->Size = System::Drawing::Size(129, 39);
			this->buttonSetHorizon->TabIndex = 41;
			this->buttonSetHorizon->Text = L"Set Horizon";
			this->buttonSetHorizon->UseVisualStyleBackColor = true;
			this->buttonSetHorizon->Click += gcnew System::EventHandler(this, &GUITest::buttonSetHorizon_Click);
			// 
			// buttonRightCont
			// 
			this->buttonRightCont->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonRightCont->Location = System::Drawing::Point(178, 378);
			this->buttonRightCont->Name = L"buttonRightCont";
			this->buttonRightCont->Size = System::Drawing::Size(45, 39);
			this->buttonRightCont->TabIndex = 40;
			this->buttonRightCont->Text = L">>";
			this->buttonRightCont->UseVisualStyleBackColor = true;
			this->buttonRightCont->Click += gcnew System::EventHandler(this, &GUITest::buttonRightCont_Click);
			// 
			// buttonUpCont
			// 
			this->buttonUpCont->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonUpCont->Location = System::Drawing::Point(117, 331);
			this->buttonUpCont->Name = L"buttonUpCont";
			this->buttonUpCont->Size = System::Drawing::Size(53, 39);
			this->buttonUpCont->TabIndex = 39;
			this->buttonUpCont->Text = L"UP";
			this->buttonUpCont->UseVisualStyleBackColor = true;
			this->buttonUpCont->Click += gcnew System::EventHandler(this, &GUITest::buttonUpCont_Click);
			// 
			// buttonDownCont
			// 
			this->buttonDownCont->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonDownCont->Location = System::Drawing::Point(117, 424);
			this->buttonDownCont->Name = L"buttonDownCont";
			this->buttonDownCont->Size = System::Drawing::Size(53, 39);
			this->buttonDownCont->TabIndex = 38;
			this->buttonDownCont->Text = L"DOWN";
			this->buttonDownCont->UseVisualStyleBackColor = true;
			this->buttonDownCont->Click += gcnew System::EventHandler(this, &GUITest::buttonDownCont_Click);
			// 
			// buttonLeftCont
			// 
			this->buttonLeftCont->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonLeftCont->Location = System::Drawing::Point(65, 378);
			this->buttonLeftCont->Name = L"buttonLeftCont";
			this->buttonLeftCont->Size = System::Drawing::Size(45, 39);
			this->buttonLeftCont->TabIndex = 37;
			this->buttonLeftCont->Text = L"<<";
			this->buttonLeftCont->UseVisualStyleBackColor = true;
			this->buttonLeftCont->Click += gcnew System::EventHandler(this, &GUITest::buttonLeftCont_Click);
			// 
			// turn_off_motors
			// 
			this->turn_off_motors->BackColor = System::Drawing::Color::Red;
			this->turn_off_motors->Location = System::Drawing::Point(446, 176);
			this->turn_off_motors->Name = L"turn_off_motors";
			this->turn_off_motors->Size = System::Drawing::Size(129, 40);
			this->turn_off_motors->TabIndex = 35;
			this->turn_off_motors->Text = L"TURN OFF MOTORS";
			this->turn_off_motors->UseVisualStyleBackColor = false;
			this->turn_off_motors->Click += gcnew System::EventHandler(this, &GUITest::turn_off_motors_Click);
			// 
			// stop
			// 
			this->stop->BackColor = System::Drawing::Color::Red;
			this->stop->ForeColor = System::Drawing::Color::Black;
			this->stop->Location = System::Drawing::Point(116, 379);
			this->stop->Name = L"stop";
			this->stop->Size = System::Drawing::Size(53, 39);
			this->stop->TabIndex = 33;
			this->stop->Text = L"STOP";
			this->stop->UseVisualStyleBackColor = false;
			this->stop->Click += gcnew System::EventHandler(this, &GUITest::stop_Click);
			// 
			// down
			// 
			this->down->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->down->Location = System::Drawing::Point(117, 220);
			this->down->Name = L"down";
			this->down->Size = System::Drawing::Size(53, 39);
			this->down->TabIndex = 32;
			this->down->Text = L"DOWN";
			this->down->UseVisualStyleBackColor = true;
			this->down->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::down_MouseDown);
			this->down->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::down_MouseUp);
			// 
			// up
			// 
			this->up->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->up->Location = System::Drawing::Point(117, 130);
			this->up->Name = L"up";
			this->up->Size = System::Drawing::Size(53, 39);
			this->up->TabIndex = 31;
			this->up->Text = L"UP";
			this->up->UseVisualStyleBackColor = true;
			this->up->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::up_MouseDown);
			this->up->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::up_MouseUp);
			// 
			// right
			// 
			this->right->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->right->Location = System::Drawing::Point(178, 175);
			this->right->Name = L"right";
			this->right->Size = System::Drawing::Size(45, 39);
			this->right->TabIndex = 30;
			this->right->Text = L">|";
			this->right->UseVisualStyleBackColor = true;
			this->right->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::right_MouseDown);
			this->right->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::right_MouseUp);
			// 
			// motors_on
			// 
			this->motors_on->BackColor = System::Drawing::Color::Lime;
			this->motors_on->Location = System::Drawing::Point(446, 130);
			this->motors_on->Name = L"motors_on";
			this->motors_on->Size = System::Drawing::Size(129, 40);
			this->motors_on->TabIndex = 28;
			this->motors_on->Text = L"START MOTORS";
			this->motors_on->UseVisualStyleBackColor = false;
			this->motors_on->Click += gcnew System::EventHandler(this, &GUITest::motors_on_Click);
			// 
			// label_y
			// 
			this->label_y->AutoSize = true;
			this->label_y->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_y->ForeColor = System::Drawing::Color::Red;
			this->label_y->Location = System::Drawing::Point(598, 181);
			this->label_y->Name = L"label_y";
			this->label_y->Size = System::Drawing::Size(110, 31);
			this->label_y->TabIndex = 27;
			this->label_y->Text = L"Motor-Y";
			// 
			// label_x
			// 
			this->label_x->AutoSize = true;
			this->label_x->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_x->ForeColor = System::Drawing::Color::Red;
			this->label_x->Location = System::Drawing::Point(598, 139);
			this->label_x->Name = L"label_x";
			this->label_x->Size = System::Drawing::Size(110, 31);
			this->label_x->TabIndex = 26;
			this->label_x->Text = L"Motor-X";
			// 
			// left
			// 
			this->left->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->left->Location = System::Drawing::Point(65, 175);
			this->left->Name = L"left";
			this->left->Size = System::Drawing::Size(45, 39);
			this->left->TabIndex = 25;
			this->left->Text = L"|<";
			this->left->UseVisualStyleBackColor = true;
			this->left->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::left_MouseDown);
			this->left->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GUITest::left_MouseUp);
			// 
			// stopTrackingButton
			// 
			this->stopTrackingButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->stopTrackingButton->Location = System::Drawing::Point(11, 424);
			this->stopTrackingButton->Name = L"stopTrackingButton";
			this->stopTrackingButton->Size = System::Drawing::Size(129, 39);
			this->stopTrackingButton->TabIndex = 49;
			this->stopTrackingButton->Text = L"Stop Tracking";
			this->stopTrackingButton->UseVisualStyleBackColor = true;
			this->stopTrackingButton->Click += gcnew System::EventHandler(this, &GUITest::stopTrackingButton_Click);
			// 
			// track_button
			// 
			this->track_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->track_button->Location = System::Drawing::Point(11, 379);
			this->track_button->Name = L"track_button";
			this->track_button->Size = System::Drawing::Size(129, 39);
			this->track_button->TabIndex = 48;
			this->track_button->Text = L"Start Tracking";
			this->track_button->UseVisualStyleBackColor = true;
			this->track_button->Click += gcnew System::EventHandler(this, &GUITest::track_button_Click);
			// 
			// vertical_autosearch
			// 
			this->vertical_autosearch->Location = System::Drawing::Point(11, 130);
			this->vertical_autosearch->Name = L"vertical_autosearch";
			this->vertical_autosearch->Size = System::Drawing::Size(141, 40);
			this->vertical_autosearch->TabIndex = 29;
			this->vertical_autosearch->Text = L"VERTICAL";
			this->vertical_autosearch->UseVisualStyleBackColor = true;
			this->vertical_autosearch->Click += gcnew System::EventHandler(this, &GUITest::vertical_autosearch_Click);
			// 
			// horizontal_autsearch
			// 
			this->horizontal_autsearch->Location = System::Drawing::Point(11, 176);
			this->horizontal_autsearch->Name = L"horizontal_autsearch";
			this->horizontal_autsearch->Size = System::Drawing::Size(141, 40);
			this->horizontal_autsearch->TabIndex = 34;
			this->horizontal_autsearch->Text = L"HORIZONTAL";
			this->horizontal_autsearch->UseVisualStyleBackColor = true;
			this->horizontal_autsearch->Click += gcnew System::EventHandler(this, &GUITest::horizontal_autsearch_Click);
			// 
			// auto_search
			// 
			this->auto_search->AutoSize = true;
			this->auto_search->Enabled = false;
			this->auto_search->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->auto_search->Location = System::Drawing::Point(7, 16);
			this->auto_search->Name = L"auto_search";
			this->auto_search->Size = System::Drawing::Size(145, 20);
			this->auto_search->TabIndex = 36;
			this->auto_search->Text = L"Motor Identification";
			// 
			// GUITest
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(906, 517);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"GUITest";
			this->Text = L"Control System For LUMBO";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &GUITest::GUITest_FormClosed);
			this->Load += gcnew System::EventHandler(this, &GUITest::GUITest_Load);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void GUITest_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	

private: System::Void motors_on_Click(System::Object^  sender, System::EventArgs^  e) {
			std::cout << "start button pressed "  << std::endl;
			std::vector<bool> returnedMotors;
			returnedMotors = servoHandler->startMotors(); 
			if(returnedMotors.at(0)) label_x->ForeColor = System::Drawing::Color::Green;
			if(returnedMotors.at(1)) label_y->ForeColor = System::Drawing::Color::Green;		 }
private: System::Void stop_Click(System::Object^  sender, System::EventArgs^  e) {
			 servoHandler->setReference(0, 0);
		 }
private: System::Void turn_off_motors_Click(System::Object^  sender, System::EventArgs^  e) {
			 servoHandler->stop();
		 }
private: System::Void vertical_autosearch_Click(System::Object^  sender, System::EventArgs^  e) {
			 servoHandler->runTFTest(VERTICAL);
		 }
private: System::Void horizontal_autsearch_Click(System::Object^  sender, System::EventArgs^  e) {
			 servoHandler->runTFTest(HORIZONTAL);
		 }


private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {

		 }



/*------------- Events for holding a mouse click and releasing on the respective buttons -------------*/
		 // First for firing down-click-event for all steering buttons (Right, Left, Up and Down)
private: System::Void right_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 servoHandler->setReference(0.5, 0);
		 }
private: System::Void left_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 servoHandler->setReference(-0.5, 0);
		 }
private: System::Void up_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 servoHandler->setReference(0, -0.5);
		 }
private: System::Void down_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 servoHandler->setReference(0, 0.5);
		 }



		 //Then for firing the up-click-event for all steering buttons (Right, Left, Up and Down)
private: System::Void right_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 std::cout << "right up" << std::endl;
			 refSignal[X] = 0.0;
			 refSignal[Y] = 0.0;
			 servoHandler->setReference(refSignal);
		 }
private: System::Void left_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 servoHandler->pauseMotors();
		 }
private: System::Void up_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 servoHandler->pauseMotors();
		 }
private: System::Void down_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

			 servoHandler->pauseMotors();
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }



/* ---------------- For clicking on the buttons to keep moving the telescope -----------------------*/
private: System::Void buttonDownCont_Click(System::Object^  sender, System::EventArgs^  e) {

			 servoHandler->setReference(0, 3);
		 }
private: System::Void buttonUpCont_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 servoHandler->setReference(0, -3);
		 }
private: System::Void buttonLeftCont_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 servoHandler->setReference(-3, 0);
		 }
private: System::Void buttonRightCont_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 servoHandler->setReference(3, 0);
		 }

		 

private: System::Void GUITest_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 servoHandler->stop();
			 imageHandler->stop();
			 Application::Exit();
		 }

private: System::Void buttonSetHorizon_Click(System::Object^  sender, System::EventArgs^  e) {
			 servoHandler->setHorizon();
		 }
private: System::Void track_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 imageHandler->startTracking();
		 }
private: System::Void stopTrackingButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 imageHandler->stopTracking();
		 }
private: System::Void splitContainer1_Panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
};

// Kopiera in denna rad och ersätt med raden ovan om programmet inte kompilerar, gör det sen för label3 nedan också
/*
this->label4->Text = L"This button will create a lower boundery \r\nfor which the Y motor will not pass. As no tracking \r\ncan be done except for in the sky; set the horizon to \r\njust above the tree line. Press UP/DOWN until the \r\ncamera is above the tree line and then press this \r\nbutton";
*/
/* --------------TA INTE BORT KOPIERA ENBART!!!!!!!!!!!!!----------------
this->label3->Text = L"Press start motors to start the process. Homefinding will be be initialized, \r\nwhich has to be done manually. If a motor is found and initialized the text \r\nwill change from red to green (Motor-X/Motor-Y). After that the motors will \r\nautomatically move from Homefinding position to the dome opening. Turn \r\nmotors will turn of the motors and can only be started again by restarting \r\nprogram! (Not the same as the stop button!!)";
*/
