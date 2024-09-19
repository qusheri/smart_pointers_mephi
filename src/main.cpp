#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTextEdit>
#include <chrono>
#include "../tests/smart_ptrs_tests.h"
#include "../tests/memory_checker.h"
#include "../tests/test_structure.h"
#include "QtCharts"
#include "QtWidgets"
#include "algorithm"
class MainWindow : public QMainWindow {
public:
    MainWindow() {
        setWindowTitle("Smart Pointer Test Menu");

        QMenu *Charts = menuBar()->addMenu("Charts");
        QMenu *shrd = menuBar()->addMenu("shared_ptrs");
        QMenu *uniq = menuBar()->addMenu("uniq_ptrs");

        QAction *runUnqPtrTestsAction = new QAction("Run UnqPtr Tests", this);
        connect(runUnqPtrTestsAction, &QAction::triggered, this, &MainWindow::runUnqPtrTestsSlot);
        uniq->addAction(runUnqPtrTestsAction);

        QAction *runShrdPtrTestsAction = new QAction("Run ShrdPtr Tests", this);
        connect(runShrdPtrTestsAction, &QAction::triggered, this, &MainWindow::runShrdPtrTestsSlot);
        shrd->addAction(runShrdPtrTestsAction);

        QAction *runLinkedListTestsAction = new QAction("Run LinkedList Tests", this);
        connect(runLinkedListTestsAction, &QAction::triggered, this, &MainWindow::runLinkedListUniqueSlot);
        uniq->addAction(runLinkedListTestsAction);

        QAction *runStdUnqPtrTestsAction = new QAction("Run std::unique_ptr Tests", this);
        connect(runStdUnqPtrTestsAction, &QAction::triggered, this, &MainWindow::runStdUnqPtrTestsSlot);
        uniq->addAction(runStdUnqPtrTestsAction);

        QAction *runStdShrdPtrTestsAction = new QAction("Run std::shared_ptr Tests", this);
        connect(runStdShrdPtrTestsAction, &QAction::triggered, this, &MainWindow::runStdShrdPtrTestsSlot);
        shrd->addAction(runStdShrdPtrTestsAction);

        QAction *showComparisonChartActionUniq = new QAction("Show Comparison Chart for Uniq ptrs", this);
        connect(showComparisonChartActionUniq, &QAction::triggered, this, &MainWindow::showComparisonChartUniq);
        Charts->addAction(showComparisonChartActionUniq);

        QAction *showComparisonChartActionShared = new QAction("Show Comparison Chart for Shared ptrs", this);
        connect(showComparisonChartActionShared, &QAction::triggered, this, &MainWindow::showComparisonChartShared);
        Charts->addAction(showComparisonChartActionShared);

        QAction *exitAction = new QAction("Exit", this);
        connect(exitAction, &QAction::triggered, this, &MainWindow::close);
        uniq->addAction(exitAction);
        shrd->addAction(exitAction);
        Charts->addAction(exitAction);
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

    void runStdUnqPtrTestsSlot() {
        QString results = runTestsWithProfiling("std::unique_ptr", runStdUnqPtrTests);
        showResults("std::unique_ptr Tests", results);
    }

    void runStdShrdPtrTestsSlot() {
        QString results = runTestsWithProfiling("std::shared_ptr", runStdShrdPtrTests);
        showResults("std::shared_ptr Tests", results);
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
        results += testFunction();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        size_t memoryUsed = measureMemoryUsage();

        results += "Time taken: " + QString::number(duration.count()) + " seconds\n";
        results += "Memory used: " + QString::number(memoryUsed) + " bytes\n";

        return results;
    }
    void showComparisonChartUniq() {
        QLineSeries *unqPtrSeries = new QLineSeries();
        unqPtrSeries->setName("UnqPtr");

        QLineSeries *stdUnqPtrSeries = new QLineSeries();
        stdUnqPtrSeries->setName("std::unique_ptr");

        auto UnqPtrTests = loadUnqPtrTests();
        auto StdUnqPtrTests = loadStdUnqPtrTests();

        for(auto test: UnqPtrTests){
            unqPtrSeries->append(test.second, test.first);
            std::cout << test.second << ' ' << test.first << '\n';
        }
        for(auto test: StdUnqPtrTests){
            stdUnqPtrSeries->append(test.second, test.first);
            std::cout << test.second << ' ' << test.first << '\n';
        }

        QChart *chart = new QChart();
        chart->addSeries(unqPtrSeries);
        chart->addSeries(stdUnqPtrSeries);
        chart->setTitle("UnqPtr vs std::unique_ptr Performance Comparison");

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("Performance Time (ms)");
        axisX->setRange(0, std::max(UnqPtrTests[UnqPtrTests.size() - 1].second, StdUnqPtrTests[StdUnqPtrTests.size() - 1].second));
        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Test Size");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        unqPtrSeries->attachAxis(axisX);
        unqPtrSeries->attachAxis(axisY);
        stdUnqPtrSeries->attachAxis(axisX);
        stdUnqPtrSeries->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *chartWindow = new QMainWindow;
        chartWindow->setCentralWidget(chartView);
        chartWindow->resize(800, 600);
        chartWindow->show();
    }
    void showComparisonChartShared() {
        QLineSeries *ShrdPtrSeries = new QLineSeries();
        ShrdPtrSeries->setName("ShrdPtr");

        QLineSeries *stdShrdPtrSeries = new QLineSeries();
        stdShrdPtrSeries->setName("std::shared_ptr");

        auto ShrdPtrTests = loadShrdPtrTests();
        auto StdShrdPtrTests = loadStdShrdPtrTests();

        for(auto test: ShrdPtrTests){
            ShrdPtrSeries->append(test.second, test.first);
            std::cout << test.second << ' ' << test.first << '\n';
        }
        for(auto test: StdShrdPtrTests){
            stdShrdPtrSeries->append(test.second, test.first);
            std::cout << test.second << ' ' << test.first << '\n';
        }

        QChart *chart = new QChart();
        chart->addSeries(ShrdPtrSeries);
        chart->addSeries(stdShrdPtrSeries);
        chart->setTitle("UnqPtr vs std::unique_ptr Performance Comparison");

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("Performance Time (ms)");
        axisX->setRange(0, std::max(ShrdPtrTests[ShrdPtrTests.size() - 1].second, StdShrdPtrTests[StdShrdPtrTests.size() - 1].second));
        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Test Size");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        ShrdPtrSeries->attachAxis(axisX);
        ShrdPtrSeries->attachAxis(axisY);
        stdShrdPtrSeries->attachAxis(axisX);
        stdShrdPtrSeries->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *chartWindow = new QMainWindow;
        chartWindow->setCentralWidget(chartView);
        chartWindow->resize(800, 600);
        chartWindow->show();
    }
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(600, 400);
    window.show();

    return app.exec();
}
