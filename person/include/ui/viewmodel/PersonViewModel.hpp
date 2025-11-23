#pragma once

#include <application/port/IPersonCommandService.hpp>
#include <application/port/IPersonQueryService.hpp>
#include <ui/viewmodel/QmlBaseViewModel.hpp>
#include <ui/viewmodel/QmlPerson.hpp>

class PersonViewModel final : public QmlBaseViewModel
{
    Q_OBJECT

public:
    explicit PersonViewModel(
        ILogger *compositeLogger,
        IPersonCommandService *personCommandService,
        IPersonQueryService *personQueryService,
        QObject *parent = nullptr);
    ~PersonViewModel() override = default;

    Q_INVOKABLE void createPerson(const QString &name);
    Q_INVOKABLE void deletePerson(long long id);
    Q_INVOKABLE QmlPerson *getPersonById(long long id) const;

protected:
    QHash<int, QByteArray> getConcreteRoleNames() const override;
    QVariant getConcreteData(const QmlBaseEntity *item, int role) const override;
    void concreteLoad() override;

private:
    IPersonCommandService *m_personCommandService;
    IPersonQueryService *m_personQueryService;

    enum PersonRoles {
        NameRole = Qt::UserRole + 2,
        ActiveRole,
    };
};
