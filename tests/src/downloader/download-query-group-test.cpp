#include "download-query-group-test.h"
#include <QtTest>
#include "downloader/download-query-group.h"
#include "models/profile.h"
#include "models/site.h"
#include "models/source.h"


void DownloadQueryGroupTest::testCompare()
{
	DownloadQueryGroup a(QStringList() << "tags", 1, 2, 3, QStringList() << "postFiltering", true, nullptr, "filename", "path");
	DownloadQueryGroup b(QStringList() << "tags", 1, 2, 3, QStringList() << "postFiltering", true, nullptr, "filename", "path");
	DownloadQueryGroup c(QStringList() << "tags", 1, 3, 3, QStringList() << "postFiltering", true, nullptr, "filename", "path");
	DownloadQueryGroup d(QStringList() << "tags", 1, 3, 3, QStringList() << "postFiltering", true, nullptr, "filename", "path");

	d.progressVal = 37;
	d.progressFinished = false;

	QVERIFY(a == b);
	QVERIFY(b == a);
	QVERIFY(a != c);
	QVERIFY(b != c);
	QVERIFY(c == c);
	QVERIFY(c == d); // The progress status must NOT be checked
}

void DownloadQueryGroupTest::testSerialization()
{
	Profile profile("tests/resources/");
	Source source(&profile, "tests/resources/sites/Danbooru (2.0)");
	Site site("danbooru.donmai.us", &source);

	DownloadQueryGroup original(QStringList() << "tags", 1, 2, 3, QStringList() << "postFiltering", true, &site, "filename", "path");
	original.progressVal = 37;
	original.progressFinished = false;

	QJsonObject json;
	original.write(json);

	DownloadQueryGroup dest;
	dest.read(json, QMap<QString, Site*> {{ site.url(), &site }});

	QCOMPARE(dest.query.tags, QStringList() << "tags");
	QCOMPARE(dest.page, 1);
	QCOMPARE(dest.perpage, 2);
	QCOMPARE(dest.total, 3);
	QCOMPARE(dest.postFiltering, QStringList() << "postFiltering");
	QCOMPARE(dest.getBlacklisted, true);
	QCOMPARE(dest.site, &site);
	QCOMPARE(dest.filename, QString("filename"));
	QCOMPARE(dest.path, QString("path"));
	QCOMPARE(dest.progressVal, 37);
	QCOMPARE(dest.progressFinished, false);
}


QTEST_MAIN(DownloadQueryGroupTest)
