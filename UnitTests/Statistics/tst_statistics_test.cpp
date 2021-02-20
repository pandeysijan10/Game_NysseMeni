#include <QtTest>
#include "../../Game/statistics.hh"

/**
  * @file UnitTest for statistics class
  * @brief tests points given by the statistics class
  */
Q_DECLARE_METATYPE(int)
class statistics_test : public QObject
{
    Q_OBJECT

public:
    statistics_test();
    ~statistics_test();

private slots:
    void points();
    void points_data();

};

statistics_test::statistics_test()
{

}

statistics_test::~statistics_test()
{

}

void statistics_test::points()
{
    QFETCH(int, points);
    StudentSide::statistics stat(points);
    QVERIFY2(stat.givePoints() == points, "Wrong points");
}

void statistics_test::points_data()
{
    QTest::addColumn<int>("points");
    QTest::newRow("hero lost")      << -100;
    QTest::newRow("hero lost")      << 0;
    QTest::newRow("hero won")      << 4000;
}


QTEST_APPLESS_MAIN(statistics_test)

#include "tst_statistics_test.moc"
