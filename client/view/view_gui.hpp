#pragma once

namespace Ui {
class ViewGUI;
}

namespace general {
namespace client {
namespace view {

/// \ref View implementation for GUI using Qt.
/// \TODO remove public
class ViewGUI : public QMainWindow {
    Q_OBJECT
public:
    /*! 
     * \param application keeps std::weak_ptr to QApplication for loop running.
     * Original std::shared_ptr to QApplication should 
     * be accessible for an entire ViewGUI lifetime.
    */
    explicit ViewGUI();
    ~ViewGUI();

private:
    Ui::ViewGUI *ui;
};

}
}
}
