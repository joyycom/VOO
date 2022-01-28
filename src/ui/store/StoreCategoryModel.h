#ifndef StoreCategoryModel_H
#define StoreCategoryModel_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>
#include <QVariantList>

struct SubCategory {
    QString cateId;
    QString cate;
    QStringList subCate;
    QStringList subCateIds;
};
Q_DECLARE_METATYPE(SubCategory)

class CategoryModel {
public:
    CategoryModel(QString id, QString title, QList<SubCategory> subCategory);

    QString id() const;
    QString title() const;
    QList<SubCategory> subCategory() const;

    void updateSubCategory(QList<SubCategory> categories);

private:
    QString m_id;
    QString m_title;
    QList<SubCategory> m_subCategory;
};

class CategoryListModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum CategoryListRole {
        IdRole = Qt::UserRole + 1,
        TitleRole = Qt::UserRole + 2,
        SubCategorysRole = Qt::UserRole + 3,
        HasSubRole = Qt::UserRole + 4,
        SelectedSubCategoryIndex = Qt::UserRole + 5,
        SelectedThirdIndex = Qt::UserRole + 6
    };

    CategoryListModel(CategoryListModel &parent);

    CategoryListModel(QObject *parent = 0);
    ~CategoryListModel();

    void addData(const SubCategory &data);
    void insertData(int index, const SubCategory &data);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setSelectedIndex(const int subCateIndex, const int thirdIndex = -1);

    void clearAll();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    int m_selectedSubIndex = 0;
    int m_selectedThirdIndex = -1;
    QList<SubCategory> datas;
};

#endif
