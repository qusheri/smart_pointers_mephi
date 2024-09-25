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

        QMenu *ptrManagement = menuBar()->addMenu("Pointer Management");

        QAction *createUnqPtrAction = new QAction("Create Unqptr", this);
        connect(createUnqPtrAction, &QAction::triggered, this, &MainWindow::createUnqPtrSlot);
        ptrManagement->addAction(createUnqPtrAction);

        QAction *rewriteUnqPtrAction = new QAction("Rewrite Unqptr", this);
        connect(rewriteUnqPtrAction, &QAction::triggered, this, &MainWindow::rewriteUnqPtrSlot);
        ptrManagement->addAction(rewriteUnqPtrAction);

        QAction *releaseUnqPtrAction = new QAction("Release Unqptr", this);
        connect(releaseUnqPtrAction, &QAction::triggered, this, &MainWindow::releaseUnqPtrSlot);
        ptrManagement->addAction(releaseUnqPtrAction);

        QAction *getvalUnqPtrAction = new QAction("Get val of Unqptr", this);
        connect(getvalUnqPtrAction, &QAction::triggered, this, &MainWindow::getvalUnqPtrSlot);
        ptrManagement->addAction(getvalUnqPtrAction);

        QAction *createShrdPtrAction = new QAction("Create shared ptr", this);
        connect(createShrdPtrAction, &QAction::triggered, this, &MainWindow::createShrdPtrSlot);
        ptrManagement->addAction(createShrdPtrAction);

        QAction *rewriteShrdPtrAction = new QAction("Rewrite shared ptr", this);
        connect(rewriteShrdPtrAction, &QAction::triggered, this, &MainWindow::rewriteShrdPtrSlot);
        ptrManagement->addAction(rewriteShrdPtrAction);

        QAction *releaseShrdPtrAction = new QAction("Release shared ptr", this);
        connect(releaseShrdPtrAction, &QAction::triggered, this, &MainWindow::releaseShrdPtrSlot);
        ptrManagement->addAction(releaseShrdPtrAction);

        QAction *getvalShrdPtrAction = new QAction("Get val of shared ptr", this);
        connect(getvalShrdPtrAction, &QAction::triggered, this, &MainWindow::getvalShrdPtrSlot);
        ptrManagement->addAction(getvalShrdPtrAction);


        
        
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
    void createUnqPtrSlot(bool for_rewrite = false) {
        bool ok;
        int value = QInputDialog::getInt(this, "Input Value", "Enter an integer value for UnqPtr:", 0, INT_MIN, INT_MAX, 1, &ok);

        if (ok) {
            uniq_ptr = UnqPtr<int>(new int(value));
            QString result = "Created UnqPtr with value: " + QString::number(*uniq_ptr);
            if(!for_rewrite) showResults("UnqPtr", result, 400, 300);
        }
    }

    void rewriteUnqPtrSlot() {
        uniq_ptr.release();
        createUnqPtrSlot(true);
        QString result = "Rewrited UnqPtr with value: " + QString::number(*uniq_ptr);
        showResults("UnqPtr", result, 400, 300);
    }

    void releaseUnqPtrSlot() {
        uniq_ptr.release();
        QString result = "UnqPtr has been released";
        showResults("UnqPtr", result, 400, 300);
    }

    void getvalUnqPtrSlot() {
        if(!uniq_ptr.null()) {
            QString result = "UnqPtr value: " + QString::number(*uniq_ptr);
            showResults("UnqPtr", result, 400, 300);
        }
        else{
            QString result = "UnqPtr value: null";
            showResults("UnqPtr", result, 400, 300);
        }
    }

    void createShrdPtrSlot(bool for_rewrite = false) {
        bool ok;
        int value = QInputDialog::getInt(this, "Input Value", "Enter an integer value for ShrdPtr:", 0, INT_MIN, INT_MAX, 1, &ok);

        if (ok) {
            shared_ptr = ShrdPtr<int>(new int(value));
            QString result = "Created ShrdPtr with value: " + QString::number(*shared_ptr);
            if(!for_rewrite) showResults("ShrdPtr", result, 400, 300);
        }
    }


    void rewriteShrdPtrSlot() {
        shared_ptr.reset();
        createShrdPtrSlot(true);
        QString result = "Rewrited ShrdPtr with value: " + QString::number(*shared_ptr);
        showResults("ShrdPtr", result, 400, 300);
    }
    
    void releaseShrdPtrSlot() {
        shared_ptr.reset();
        QString result = "ShrdPtr has been released";
        showResults("ShrdPtr", result, 400, 300);
    }
    
    void getvalShrdPtrSlot() {
        if(!shared_ptr.null()) {
            QString result = "ShrdPtr value: " + QString::number(*shared_ptr);
            showResults("ShrdPtr", result, 400, 300);
        }
        else{
            QString result = "ShrdPtr value: null";
            showResults("ShrdPtr", result, 400, 300);
        }
    }

    void showResults(const QString& title, const QString& results, int w, int h) {
        QTextEdit *textEdit = new QTextEdit;
        textEdit->setPlainText(results);
        textEdit->setReadOnly(true);

        QMainWindow *resultsWindow = new QMainWindow;
        resultsWindow->setWindowTitle(title);
        resultsWindow->setCentralWidget(textEdit);
        resultsWindow->resize(w, h);
        resultsWindow->show();
    }

    void runUnqPtrTestsSlot() {
        QString results = runTestsWithProfiling("UnqPtr", runUnqPtrTests);
        showResults("UnqPtr Tests", results, 600, 400);
    }

    void runShrdPtrTestsSlot() {
        QString results = runTestsWithProfiling("ShrdPtr", runShrdPtrTests);
        showResults("ShrdPtr Tests", results, 600, 400);
    }

    void runLinkedListUniqueSlot() {
        QString results = runTestsWithProfiling("LinkedList", runLinkedListUniqueTests);
        showResults("LinkedList Tests", results, 600, 400);
    }

    void runStdUnqPtrTestsSlot() {
        QString results = runTestsWithProfiling("std::unique_ptr", runStdUnqPtrTests);
        showResults("std::unique_ptr Tests", results, 600, 400);
    }

    void runStdShrdPtrTestsSlot() {
        QString results = runTestsWithProfiling("std::shared_ptr", runStdShrdPtrTests);
        showResults("std::shared_ptr Tests", results, 600, 400);
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

        QScatterSeries *unqPtrScatterSeries = new QScatterSeries();
        unqPtrScatterSeries->setName("UnqPtr Points");
        unqPtrScatterSeries->setMarkerSize(8);

        QLineSeries *stdUnqPtrSeries = new QLineSeries();
        stdUnqPtrSeries->setName("std::unique_ptr");

        QScatterSeries *stdUnqPtrScatterSeries = new QScatterSeries();
        stdUnqPtrScatterSeries->setName("std::unique_ptr Points");
        stdUnqPtrScatterSeries->setMarkerSize(8);

        auto tests = Unq_tests();

        auto UnqPtrTests = tests[0];
        auto StdUnqPtrTests = tests[1];

        for(auto test: UnqPtrTests) {
            unqPtrSeries->append(test.second, test.first);
            unqPtrScatterSeries->append(test.second, test.first);
        }
        for(auto test: StdUnqPtrTests) {
            stdUnqPtrSeries->append(test.second, test.first);
            stdUnqPtrScatterSeries->append(test.second, test.first);
        }

        QChart *chart = new QChart();
        chart->addSeries(unqPtrSeries);
        chart->addSeries(unqPtrScatterSeries);
        chart->addSeries(stdUnqPtrSeries);
        chart->addSeries(stdUnqPtrScatterSeries);
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
        unqPtrScatterSeries->attachAxis(axisX);
        unqPtrScatterSeries->attachAxis(axisY);
        stdUnqPtrSeries->attachAxis(axisX);
        stdUnqPtrSeries->attachAxis(axisY);
        stdUnqPtrScatterSeries->attachAxis(axisX);
        stdUnqPtrScatterSeries->attachAxis(axisY);

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

        QScatterSeries *ShrdPtrScatterSeries = new QScatterSeries();
        ShrdPtrScatterSeries->setName("ShrdPtr Points");
        ShrdPtrScatterSeries->setMarkerSize(8);

        QLineSeries *stdShrdPtrSeries = new QLineSeries();
        stdShrdPtrSeries->setName("std::shared_ptr");

        QScatterSeries *stdShrdPtrScatterSeries = new QScatterSeries();
        stdShrdPtrScatterSeries->setName("std::shared_ptr Points");
        stdShrdPtrScatterSeries->setMarkerSize(8);

        auto tests = Shrd_tests();

        auto ShrdPtrTests = tests[0];
        auto StdShrdPtrTests = tests[1];

        for(auto test: ShrdPtrTests) {
            ShrdPtrSeries->append(test.second, test.first);
            ShrdPtrScatterSeries->append(test.second, test.first);
        }
        for(auto test: StdShrdPtrTests) {
            stdShrdPtrSeries->append(test.second, test.first);
            stdShrdPtrScatterSeries->append(test.second, test.first);
        }

        QChart *chart = new QChart();
        chart->addSeries(ShrdPtrSeries);
        chart->addSeries(ShrdPtrScatterSeries);
        chart->addSeries(stdShrdPtrSeries);
        chart->addSeries(stdShrdPtrScatterSeries);
        chart->setTitle("ShrdPtr vs std::shared_ptr Performance Comparison");

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("Performance Time (ms)");
        axisX->setRange(0, std::max(ShrdPtrTests[ShrdPtrTests.size() - 1].second, StdShrdPtrTests[StdShrdPtrTests.size() - 1].second));
        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Test Size");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        ShrdPtrSeries->attachAxis(axisX);
        ShrdPtrSeries->attachAxis(axisY);
        ShrdPtrScatterSeries->attachAxis(axisX);
        ShrdPtrScatterSeries->attachAxis(axisY);
        stdShrdPtrSeries->attachAxis(axisX);
        stdShrdPtrSeries->attachAxis(axisY);
        stdShrdPtrScatterSeries->attachAxis(axisX);
        stdShrdPtrScatterSeries->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QMainWindow *chartWindow = new QMainWindow;
        chartWindow->setCentralWidget(chartView);
        chartWindow->resize(800, 600);
        chartWindow->show();
    }

private:
    std::vector<std::vector<std::pair<int, long long>>> Shrd_tests(){
        auto ShrdPtrTests = loadShrdPtrTests();
        auto StdShrdPtrTests = loadStdShrdPtrTests();
        return {ShrdPtrTests,StdShrdPtrTests};
    }
    std::vector<std::vector<std::pair<int, long long>>> Unq_tests(){
        auto UnqPtrTests = loadUnqPtrTests();
        auto StdUnqPtrTests = loadStdUnqPtrTests();
        return {UnqPtrTests, StdUnqPtrTests};
    }
    UnqPtr<int> uniq_ptr;
    ShrdPtr<int> shared_ptr;
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(600, 400);
    window.show();

    return app.exec();
}
