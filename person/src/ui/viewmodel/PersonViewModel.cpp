#include <algorithm>
#include <ui/viewmodel/PersonViewModel.hpp>
#include <QList>

PersonViewModel::PersonViewModel(
    ILogger *compositeLogger,
    IPersonCommandService *personCommandService,
    IPersonQueryService *personQueryService,
    QObject *parent)
    : QmlBaseViewModel(compositeLogger, parent)
    , m_personCommandService(personCommandService)
    , m_personQueryService(personQueryService)
{}

QHash<int, QByteArray> PersonViewModel::getConcreteRoleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ActiveRole] = "active";
    return roles;
}

QVariant PersonViewModel::getConcreteData(const QmlBaseEntity *item, const int role) const
{
    const auto person = qobject_cast<const QmlPerson *>(item);
    if (!person) {
        qWarning()
            << "PersonViewModel::getConcreteData() - Failed to cast QmlBaseEntity to QmlPerson.";
        return QVariant{};
    }

    switch (role) {
    case IdRole:
        return person->id();
    case NameRole:
        return person->name();
    case ActiveRole:
        return person->active();
    case Qt::DisplayRole:
        return person->name();
    default:
        qDebug() << "PersonViewModel::getConcreteData() Unknown role:" << role;
        return QVariant{};
    }
}

void PersonViewModel::concreteLoad()
{
    QList<QmlBaseEntity *> newPersons;
    try {
        if (const auto persons = m_personQueryService->findAll(FindAllPersonQuery());
            !persons.empty()) {
            for (const auto &person : persons) {
                newPersons.append(new QmlPerson(person, this));
            }
        }
    } catch (const std::exception &e) {
        qWarning() << "Failed to load persons from repository:" << e.what();
        emit operationFailed(QString("Failed to load persons: %1").arg(e.what()));
    }
    resetData(newPersons);
}

void PersonViewModel::createPerson(const QString &name)
{
    executeSafe([this, name]() {
        m_personCommandService->execute(CreatePersonCommand{name.toStdString(), true});
        load();
    });
}

void PersonViewModel::deletePerson(const long long id)
{
    const auto it = std::ranges::find_if(m_data, [id](const QPointer<QmlBaseEntity> &item) {
        if (const auto person = qobject_cast<const QmlPerson *>(item.data())) {
            return person->id() == id;
        }
        return false;
    });

    if (it != m_data.end()) {
        executeSafe([this, it, id]() {
            const std::ptrdiff_t row = std::distance(m_data.begin(), it);
            beginRemoveRows(QModelIndex(), static_cast<int>(row), static_cast<int>(row));
            m_personCommandService->execute(DeletePersonCommand{id});
            m_data.erase(it);
            endRemoveRows();
        });
    } else {
        executeSafe([this, id]() { m_personCommandService->execute(DeletePersonCommand{id}); });
    }
}

QmlPerson *PersonViewModel::getPersonById(const long long id) const
{
    return getById<QmlPerson>(id);
}
