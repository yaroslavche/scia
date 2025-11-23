#pragma once

#include <application/port/ILogger.hpp>
#include <functional>
#include <ui/qml/QmlBaseEntity.hpp>
#include <QAbstractListModel>
#include <QList>
#include <QPointer>

class QmlBaseViewModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged FINAL)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged FINAL)
    Q_PROPERTY(QVariantMap validationErrors READ validationErrors NOTIFY validationErrorsChanged)

public:
    explicit QmlBaseViewModel(ILogger *logger, QObject *parent = nullptr);
    ~QmlBaseViewModel() override;

    enum BaseRoles { IdRole = Qt::UserRole + 1 };

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void load();
    Q_INVOKABLE QmlBaseEntity *get(int index) const;

    [[nodiscard]] bool isLoading() const { return m_isLoading; }
    [[nodiscard]] QString errorMessage() const { return m_errorMessage; }
    [[nodiscard]] QVariantMap validationErrors() const { return m_validationErrors; }

signals:
    void isLoadingChanged();
    void errorMessageChanged();
    void operationSucceeded(const QString &message);
    void operationFailed(const QString &errorMessage);
    void validationErrorsChanged();

protected:
    QList<QPointer<QmlBaseEntity>> m_data;
    mutable QHash<int, QByteArray> m_roleNamesCache; // mutable is ok for roleNamesCache
    QVariantMap m_validationErrors;
    ILogger *m_logger;

    virtual void concreteLoad() = 0;
    virtual QVariant getConcreteData(const QmlBaseEntity *item, int role) const = 0;
    [[nodiscard]] virtual QHash<int, QByteArray> getConcreteRoleNames() const = 0;
    void setIsLoading(bool loading);
    void setErrorMessage(const QString &message);
    void appendItem(QmlBaseEntity *item);
    void appendItems(const QList<QmlBaseEntity *> &items);
    void resetData(const QList<QmlBaseEntity *> &newData);
    bool updateItem(
        long long id,
        const std::function<void(QmlBaseEntity *)> &updateFn,
        const QList<int> &rolesToNotify = {});
    void setValidationErrors(const QVariantMap &errors);
    void clearValidationErrors();
    void executeSafe(const std::function<void()>& action);

    template<typename T>
    requires std::derived_from<T, QmlBaseEntity> T *getById(const qlonglong id) const
    {
        if (id < 1)
            return nullptr;

        const auto it = std::ranges::find_if(m_data, [id](const QPointer<QmlBaseEntity> &item) {
            if (const auto qmlItem = qobject_cast<T *>(item.data()))
                return qmlItem->id() == id;
            return false;
        });
        if (it != m_data.end())
            return qobject_cast<T *>(it->data());
        return nullptr;
    }

private:
    bool m_isLoading;
    QString m_errorMessage;
};
