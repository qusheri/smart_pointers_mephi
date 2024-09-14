#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTextEdit>
#include <chrono>
#include "../tests/smart_ptrs_tests.h"
#include "../tests/memory_checker.h"
#include "../tests/test_structure.h"
class MainWindow : public QMainWindow {
public:
    MainWindow() {
        setWindowTitle("Smart Pointer Test Menu");

        QMenu *fileMenu = menuBar()->addMenu("Tests");

        QAction *runUnqPtrTestsAction = new QAction("Run UnqPtr Tests", this);
        connect(runUnqPtrTestsAction, &QAction::triggered, this, &MainWindow::runUnqPtrTestsSlot);
        fileMenu->addAction(runUnqPtrTestsAction);

        QAction *runShrdPtrTestsAction = new QAction("Run ShrdPtr Tests", this);
        connect(runShrdPtrTestsAction, &QAction::triggered, this, &MainWindow::runShrdPtrTestsSlot);
        fileMenu->addAction(runShrdPtrTestsAction);

        QAction *runLinkedListTestsAction = new QAction("Run LinkedList Tests", this);
        connect(runLinkedListTestsAction, &QAction::triggered, this, &MainWindow::runLinkedListUniqueSlot);
        fileMenu->addAction(runLinkedListTestsAction);

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

    void runLinkedListUniqueSlot() {
        QString results = runTestsWithProfiling("LinkedList", runLinkedListUniqueTests);
        showResults("LinkedList Tests", results);
    }


    void showResults(const QString& title, const QString& results) {
        QTextEdit *textEdit = new QTextEdit;
        textEdit->setPlainText(results);
        textEdit->setReadOnly(true);

        QMainWindow *resultsWindow = new QMainWindow;
        resultsWindow->setWindowTitle(title);
        resultsWindow->setCentralWidget(textEdit);
        resultsWindow->resize(600, 400);
        resultsWindow->show();
    }

    template<typename Func>
    QString runTestsWithProfiling(const QString& testName, Func testFunction) {
        QString results;
        results += "Running " + testName + " tests...\n";

        auto start = std::chrono::high_resolution_clock::now();
        testFunction();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

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
