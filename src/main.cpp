#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextEdit>
#include <chrono>
#include "../tests/smart_ptrs_tests.h"
#include "../tests/memory_checker.h"

class MainWindow : public QMainWindow {
public:
    MainWindow() {
        setWindowTitle("Smart Pointer Test Menu");

        // Создаем меню
        QMenu *fileMenu = menuBar()->addMenu("Tests");

        // Пункт меню для теста производительности UnqPtr
        QAction *runUnqPtrTestsAction = new QAction("Run UnqPtr Tests", this);
        connect(runUnqPtrTestsAction, &QAction::triggered, this, &MainWindow::runUnqPtrTestsSlot);
        fileMenu->addAction(runUnqPtrTestsAction);

        // Пункт меню для теста производительности ShrdPtr
        QAction *runShrdPtrTestsAction = new QAction("Run ShrdPtr Tests", this);
        connect(runShrdPtrTestsAction, &QAction::triggered, this, &MainWindow::runShrdPtrTestsSlot);
        fileMenu->addAction(runShrdPtrTestsAction);


        // Пункт меню для выхода
        QAction *exitAction = new QAction("Exit", this);
        connect(exitAction, &QAction::triggered, this, &MainWindow::close);
        fileMenu->addAction(exitAction);
    }

private slots:
    void runUnqPtrTestsSlot() {
        QString results = runTestsWithProfiling("UnqPtr", runUnqPtrTests);
        showResults("UnqPtr Tests", results);
    }

    void runShrdPtrTestsSlot() {
        QString results = runTestsWithProfiling("ShrdPtr", runShrdPtrTests);
        showResults("ShrdPtr Tests", results);
    }

    void showResults(const QString& title, const QString& results) {
        // Окно с результатами
        QTextEdit *textEdit = new QTextEdit;
        textEdit->setPlainText(results);
        textEdit->setReadOnly(true);

        QMainWindow *resultsWindow = new QMainWindow;
        resultsWindow->setWindowTitle(title);
        resultsWindow->setCentralWidget(textEdit);
        resultsWindow->resize(600, 400);
        resultsWindow->show();
    }

    // Функция для запуска тестов с профилированием
    template<typename Func>
    QString runTestsWithProfiling(const QString& testName, Func testFunction) {
        QString results;
        results += "Running " + testName + " tests...\n";

        // Профилирование времени
        auto start = std::chrono::high_resolution_clock::now();
        testFunction();  // Запуск тестовой функции
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        // Профилирование памяти
        size_t memoryUsed = measureMemoryUsage();

        results += "Time taken: " + QString::number(duration.count()) + " seconds\n";
        results += "Memory used: " + QString::number(memoryUsed) + " bytes\n";

        return results;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(600, 400);
    window.show();

    return app.exec();
}
