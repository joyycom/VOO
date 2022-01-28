#include "StoreCategoryModel.h"

#include <QDebug>

CategoryModel::CategoryModel(QString id, QString title, QList<SubCategory> subCategory)
    : m_id(id),
      m_title(title),
      m_subCategory(subCategory) {
}

QString CategoryModel::id() const {
    return m_id;
}

QString CategoryModel::title() const {
    return m_title;
}

QList<SubCategory> CategoryModel::subCategory() const {
    return m_subCategory;
}

void CategoryModel::updateSubCategory(QList<SubCategory> categories) {
    m_subCategory.clear();
    m_subCategory.append(categories);
}

CategoryListModel::CategoryListModel(QObject *parent) : QAbstractListModel(parent) {
}

CategoryListModel::~CategoryListModel() {
    datas.clear();
}

CategoryListModel::CategoryListModel(CategoryListModel &model) : QAbstractListModel(model.parent()) {
    datas.append(model.datas);
}

void CategoryListModel::setSelectedIndex(const int subCateIndex, const int thirdIndex) {
    int preSubIndex = m_selectedSubIndex;

    m_selectedSubIndex = subCateIndex;
    m_selectedThirdIndex = thirdIndex;

    QVector<int> vector;
    vector.append(SelectedThirdIndex);
    vector.append(SelectedSubCategoryIndex);
    vector.append(SubCategorysRole);
    QModelIndex preChangedIndex = index(preSubIndex);
    emit dataChanged(preChangedIndex, preChangedIndex, vector);

    QModelIndex changedIndex = index(subCateIndex);
    emit dataChanged(changedIndex, changedIndex, vector);
}

void CategoryListModel::addData(const SubCategory &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    datas << data;
    endInsertRows();
}

void CategoryListModel::insertData(int index, const SubCategory &data) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    datas.insert(index, data);
    endInsertRows();
}

int CategoryListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return datas.size();
}

QVariant CategoryListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= datas.count())
        return QVariant();

    SubCategory data = datas[index.row()];
    if (role == IdRole)
        return data.cateId;
    else if (role == TitleRole)
        return data.cate;
    else if (role == SubCategorysRole) {
        return data.subCate;
    } else if (role == HasSubRole) {
        return data.subCate.count() != 0;
        ;
    } else if (role == SelectedSubCategoryIndex) {
        return m_selectedSubIndex;
    } else if (role == SelectedThirdIndex) {
        return m_selectedThirdIndex;
    }

    return QVariant();
}

QHash<int, QByteArray> CategoryListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "title";
    roles[SubCategorysRole] = "subCategorys";
    roles[HasSubRole] = "hasSub";
    roles[SelectedSubCategoryIndex] = "selectedSubIndex";
    roles[SelectedThirdIndex] = "selectedThirdIndex";
    return roles;
}

void CategoryListModel::clearAll() {
    beginResetModel();
    datas.clear();
    endResetModel();
}
