#pragma once

#include <QMessageBox>

class Error {
public:
    Error() {}

    void throwError(const std::string& input, int status_code = CLOSED, QWidget* parent = nullptr) {
        status Status = static_cast<status>(status_code);
        QMessageBox errorBox(parent);
        //errorBox.setStyleSheet(" QLabel { color: white; } ");
        errorBox.setText(QString::fromStdString(input));
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setWindowTitle("Error");
        errorBox.setWindowIcon(QIcon(":/ReDPad.ico"));
        errorBox.setStandardButtons(QMessageBox::Ok);
        errorBox.exec();
        qDebug() << "Error: " << QString::fromStdString(input) << '\n';
        if(Status != OPENED) {
            exit(EXIT_FAILURE);
        }
    }

    void operator()(const std::string& input, int status_code = CLOSED, QWidget* parent = nullptr) {
        throwError(input, status_code, parent);
    }

    struct ProgramStatus {
        static const int OPEN = 1;
        static const int CLOSE = 0;
    };
private:
    enum status { CLOSED, OPENED };
};
