#include "mathlib.h"
#include <gtkmm.h>

#include <string>
#include <vector>
#include <iostream>


namespace gtk = Gtk;
namespace glib = Glib;

class WindowMain : public gtk::Window {
public:
  WindowMain() {
    add(scrolledWindow);
    scrolledWindow.add(fixed);
    
    int start_y = 35;
    int h_y = 35;

    function_entry_vec = std::vector<gtk::Entry>(num_of_entry);
    coef_entry_vec = std::vector<gtk::Entry>(num_of_entry);
    const auto la = [&](){
      if(last_entry+1 < num_of_entry)
      {
        function_entry_vec[++last_entry].show();
        coef_entry_vec[last_entry].show();
      }
    };

    for(int i = 0; i < num_of_entry; ++i){
      function_entry_vec[i].signal_activate().connect(la);
      function_entry_vec[i].set_size_request(250,30);
      fixed.add(coef_entry_vec[i]);
      fixed.add(function_entry_vec[i]);
      fixed.move(function_entry_vec[i], 20, start_y + i * h_y);
      fixed.move(coef_entry_vec[i], 300, start_y + i * h_y);
    }

    functions_label.set_label("f(x)");
    fixed.add(functions_label);
    fixed.move(functions_label, 125, 10);

    params_label.set_label("Weight");
    fixed.add(params_label);
    fixed.move(params_label, 355, 10);

    range_str_label.set_label("Range");
    fixed.add(range_str_label);
    fixed.move(range_str_label, 665, 10);

    pointx_label.set_label("Min at x: ");
    fixed.add(pointx_label);
    fixed.move(pointx_label, 600, 345);
    pointy_label.set_label("Min at y: ");
    fixed.add(pointy_label);
    fixed.move(pointy_label, 600, 380);

    value_label.set_label("Function value: ");
    fixed.add(value_label);
    fixed.move(value_label, 600, 415);

    num_of_iter_label.set_label("Number of iteration: ");
    fixed.add(num_of_iter_label);
    fixed.move(num_of_iter_label, 600, 450);

    fixed.add(button1);
    fixed.move(button1, 20, 450);
    
    range = std::vector<gtk::Entry>(4);
    for (int i = 0; i < range.size(); ++i) {
      fixed.add(range[i]);
      fixed.move(range[i], 600, start_y + h_y * i);
    }


    button1.set_label("Calculate");
    button1.signal_button_release_event().connect([&](GdkEventButton*) {
      std::string str_arg = "x,y";
      std::vector<std::string> func_vec;
      std::vector<double> coef_vec;
      std::vector<std::pair<double,double>> func_range;

      if(range[0].get_text() == "" || range[1].get_text() == "") return true;
      
      for(int i = 0;  i < range.size()/2; ++i)
      {
        double x0 = std::stod(range[2 * i].get_text()), x1 = std::stod(range[2 * i + 1].get_text());
        func_range.push_back(std::make_pair(x0,x1));
      }
      

      for(int i = 0; i < function_entry_vec.size(); ++i){
        std::string tmp_string(function_entry_vec[i].get_text());
        std::string coef(coef_entry_vec[i].get_text());

        if(tmp_string != "" && coef != ""){
          func_vec.push_back(tmp_string);
          coef_vec.push_back(std::stod(coef));
        }
      }
      if(func_vec.size() == 0) return true;
      
      if(mutex) return false;
      mutex = true;
      FunctionContainer MO_method(func_vec, str_arg, func_range, 1e-3, 0.01);
      auto res = MO_method.Convolution(coef_vec);

      num_of_iter_label.set_label("Number of iteration: " + std::to_string(res.num_iteration));
      pointx_label.set_label("Min at x: " + std::to_string(res.return_point[0]));
      pointy_label.set_label("Min at y: " + std::to_string(res.return_point[1]));
      value_label.set_label("Function value: " + std::to_string(res.value));
      
      MO_method.DrawPlot(coef_vec,"Conv");
      mutex = false;
      return true;
      });

    
    
    set_title("Program");
    resize(800, 500);
    show_all();
    for(auto& c : function_entry_vec)
      c.hide();
    for(auto& c : coef_entry_vec)
      c.hide();
    function_entry_vec[0].show();
    coef_entry_vec[0].show();
  }
  
private:
  gtk::Fixed fixed;
  gtk::ScrolledWindow scrolledWindow;
  gtk::Button button1;
  std::vector<gtk::Entry> function_entry_vec;
  std::vector<gtk::Entry> coef_entry_vec;
  std::vector<gtk::Entry> range;
  gtk::Label num_of_iter_label;
  gtk::Label pointx_label;
  gtk::Label pointy_label;
  gtk::Label value_label;
  gtk::Label functions_label;
  gtk::Label params_label;
  gtk::Label range_str_label;
  int num_of_entry = 10;
  int last_entry = 0;
  int button1Clicked = 0;
  int button2Clicked = 0;
  bool mutex = false;
};

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv);
  WindowMain window;

  return app->run(window);
}