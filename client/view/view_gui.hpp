#pragma once
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileDialog>

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

    /// Required to filter drop events.
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

public slots:
    void send_button_clicked();
    void select_file_button_clicked();

private:
    void set_if_accessible(QString filepath);
    QString ask_file_explorer();

    QString selected_file_;
    Ui::ViewGUI *ui;
};

}
}
}
