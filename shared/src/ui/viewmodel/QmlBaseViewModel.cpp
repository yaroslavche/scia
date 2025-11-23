#include <ui/viewmodel/QmlBaseViewModel.hpp>

QmlBaseViewModel::QmlBaseViewModel(ILogger *logger, QObject *parent)
    : QAbstractListModel(parent)
    , m_logger(logger)
    , m_isLoading(false)
    , m_errorMessage("")
{}

QmlBaseViewModel::~QmlBaseViewModel()
{
    for (const QPointer<QmlBaseEntity> &item : m_data) {
        if (item) {
            item->deleteLater();
        }
    }
    m_data.clear();
}

int QmlBaseViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_data.size());
}

QVariant QmlBaseViewModel::data(const QModelIndex &index, const int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_data.size()) {
        return QVariant{};
    }

    const QmlBaseEntity *item = m_data.at(index.row());
    if (!item) {
        return QVariant{};
    }

    return getConcreteData(item, role);
}

QHash<int, QByteArray> QmlBaseViewModel::roleNames() const
{
    if (m_roleNamesCache.isEmpty()) {
        m_roleNamesCache.insert(IdRole, "id");
        const QHash<int, QByteArray> concreteRoles = getConcreteRoleNames();
        for (auto it = concreteRoles.constBegin(); it != concreteRoles.constEnd(); ++it) {
            m_roleNamesCache.insert(it.key(), it.value());
        }
    }
    return m_roleNamesCache;
}

void QmlBaseViewModel::clear()
{
    if (m_data.isEmpty()) {
        return;
    }
    beginRemoveRows(QModelIndex(), 0, static_cast<int>(m_data.size()) - 1);
    for (const QPointer<QmlBaseEntity> &item : m_data) {
        if (item) {
            item->deleteLater();
        }
    }
    m_data.clear();
    endRemoveRows();
}

void QmlBaseViewModel::load()
{
    setIsLoading(true);
    setErrorMessage("");
    try {
        concreteLoad();
    } catch (const std::exception &e) {
        qWarning() << "QmlBaseViewModel::load() error:" << e.what();
        setErrorMessage(QString("Operation failed: %1").arg(e.what()));
    }
    setIsLoading(false);
}

QmlBaseEntity *QmlBaseViewModel::get(int index) const
{
    if (index >= 0 && index < m_data.size()) {
        return m_data.at(index);
    }
    return nullptr;
}

void QmlBaseViewModel::setIsLoading(const bool loading)
{
    if (m_isLoading != loading) {
        m_isLoading = loading;
        emit isLoadingChanged();
    }
}

void QmlBaseViewModel::setErrorMessage(const QString &message)
{
    if (m_errorMessage != message) {
        m_errorMessage = message;
        emit errorMessageChanged();
    }
}

void QmlBaseViewModel::appendItem(QmlBaseEntity *item)
{
    if (!item) {
        return;
    }
    beginInsertRows(QModelIndex(), static_cast<int>(m_data.size()), static_cast<int>(m_data.size()));
    m_data.append(item);
    endInsertRows();
}

void QmlBaseViewModel::appendItems(const QList<QmlBaseEntity *> &items)
{
    if (items.isEmpty()) {
        return;
    }
    beginInsertRows(
        QModelIndex(),
        static_cast<int>(m_data.size()),
        static_cast<int>(m_data.size()) + static_cast<int>(items.size()) - 1);
    for (QmlBaseEntity *item : items) {
        if (item) {
            m_data.append(item);
        }
    }
    endInsertRows();
}

void QmlBaseViewModel::resetData(const QList<QmlBaseEntity *> &newData)
{
    beginResetModel();
    for (const QPointer<QmlBaseEntity> &item : m_data) {
        if (item) {
            item->deleteLater();
        }
    }
    m_data.clear();
    for (QmlBaseEntity *item : newData) {
        if (item) {
            m_data.append(item);
        }
    }
    endResetModel();
}

bool QmlBaseViewModel::updateItem(
    const long long id,
    const std::function<void(QmlBaseEntity *)> &updateFn,
    const QList<int> &rolesToNotify)
{
    int row = 0;
    for (const QPointer<QmlBaseEntity> &itemPointer : m_data) {
        if (itemPointer) {
            if (QmlBaseEntity *item = itemPointer.data(); item && item->id() == id) {
                updateFn(item);
                const QVector<int> rolesVector = rolesToNotify.isEmpty()
                                                     ? roleNames().keys().toVector()
                                                     : rolesToNotify.toVector();
                emit dataChanged(this->index(row, 0), this->index(row, 0), rolesVector);
                return true;
            }
        }
        row++;
    }
    return false;
}

void QmlBaseViewModel::setValidationErrors(const QVariantMap &errors)
{
    if (m_validationErrors != errors) {
        m_validationErrors = errors;
        emit validationErrorsChanged();
    }
}

void QmlBaseViewModel::clearValidationErrors()
{
    if (!m_validationErrors.isEmpty()) {
        m_validationErrors.clear();
        emit validationErrorsChanged();
    }
}

void QmlBaseViewModel::executeSafe(const std::function<void()> &action)
{
    setIsLoading(true);
    setErrorMessage("");
    try {
        action();
        emit operationSucceeded("Success");
    } catch (const std::exception &ex) {
        m_logger->error(ex.what());
        setErrorMessage(QString::fromStdString(ex.what()));
        emit operationFailed(errorMessage());
    } catch (...) {
        m_logger->fatal("Unknown error");
        setErrorMessage("Unknown critical error");
        emit operationFailed(errorMessage());
    }
    setIsLoading(false);
}
