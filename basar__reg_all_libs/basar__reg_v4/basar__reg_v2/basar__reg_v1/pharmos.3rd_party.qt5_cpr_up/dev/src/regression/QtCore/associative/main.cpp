#include <QCoreApplication>
#include <QHash>
#include <QDebug>
#include <QAssociativeIterable>

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  QHash<int, QString> mapping;
  mapping.insert(7, "Seven");
  mapping.insert(11, "Eleven");
  mapping.insert(42, "Forty-two");

  QVariant variant = QVariant::fromValue(mapping);
  if (variant.canConvert<QVariantHash>()) {
    QAssociativeIterable iterable = variant.value<QAssociativeIterable>();
    // Can use foreach over the values:
    foreach (const QVariant &v, iterable) {
      qDebug() << v;
    }
    // Can use C++11 range-for over the values:
    for (const QVariant &v : iterable) {
      qDebug() << v;
    }
    // Can use iterators:
    QAssociativeIterable::const_iterator it = iterable.begin();
    const QAssociativeIterable::const_iterator end = iterable.end();
    for ( ; it != end; ++it) {
      qDebug() << *it; // The current value
      qDebug() << it.key();
      qDebug() << it.value();
    }
  }
  return 0;
}
