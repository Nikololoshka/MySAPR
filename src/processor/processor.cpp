#include "processor.h"
#include "linearMethods.h"

Processor::Processor()
{
}

void Processor::compute(const QVector<Rod> &rods,
                        const QVector<LongitudinalForce> &longitudinalForces,
                        const QVector<SectionalForce> &sectionalForces,
                        const QVector<bool> &pillars)
{
    const int COUNT = rods.size() + 1;
    const int K_SIZE = 2;

    QVector<QVector<CalcType>> A(COUNT, QVector<CalcType>(COUNT, 0));
    for (int p = 0; p < COUNT; ++p) {
        if (p < rods.size()) {
            for (int i = 0; i < K_SIZE; ++i) {
                for (int j = 0; j < K_SIZE; ++j) {
                    A[p + i][p + j] += ((i + j) % 2 == 0 ? 1 : -1) *
                            rods[p].elasticUnit() * rods[p].sectionalArea() / rods[p].length();
                    // qDebug() << QString("i: %1; j %2; value: %3").arg(p + i).arg(p + j).arg(A[p + i][p + j]);
                }
            }
        }

        if (pillars[p]) {
            for (int i = 0; i < COUNT; ++i) {
                A[i][p] = 0;
                A[p][i] = 0;
            }
            A[p][p] = 1;
        }
    }

    rods_ = rods;
    longitudinalForces_ = longitudinalForces;
    pillars_ = pillars;

    QMap<int, QPair<CalcType, CalcType>> qs;
    for (const auto &f : sectionalForces) {
        CalcType force = - f.force() * rods[f.rodId() - 1].length() / 2;
        qs[f.rodId() - 1] = {force, force};
        sectionalForces_[f.rodId() - 1] = f;
    }

    QMap<int, CalcType> fs;
    for (const auto &f : longitudinalForces) {
        fs[f.nodeId() - 1] = f.force();
    }

    QVector<CalcType> b(COUNT, 0);
    for (int i = 0; i < COUNT; ++i) {
        // F
        b[i] += fs[i];

        // q
        b[i] -= qs[i].first;
        if (i + 1 < COUNT) {
            b[i + 1] -= qs[i].second;
        }

        if (pillars[i]) {
            b[i] = 0;
        }
    }

    result_ = LinearMethods::gauss(A, b);
}

void Processor::save(QString path)
{

    const QString SUFFIX = ".json";
    const QString FULL_SUFFIC = ".compute.json";

    if (path.endsWith(SUFFIX)) {
       path.chop(SUFFIX.size());
    }

    if (!path.endsWith(FULL_SUFFIC)) {
        path += FULL_SUFFIC;
    }

    QJsonArray rodArray;
    for (auto& rod : rods_) {
        rodArray.append(rod.save());
    }
    QJsonArray sectionalForcesArray;
    for (auto& force : sectionalForces_) {
        sectionalForcesArray.append(force.save());
    }
    QJsonArray longitudinalForcesArray;
    for (auto& force : longitudinalForces_) {
        longitudinalForcesArray.append(force.save());
    }
    QJsonArray pillarsArray;
    for (auto& pillar : pillars_) {
        pillarsArray.append(QJsonValue(pillar));
    }
    QJsonArray resultsArray;
    for (auto& res : result_) {
        resultsArray.append(QJsonValue(res));
    }

    QJsonObject mainObj;
    mainObj["rods"] = rodArray;
    mainObj["sectionalForces"] = sectionalForcesArray;
    mainObj["longitudinalForces"] = longitudinalForcesArray;
    mainObj["pillars"] = pillarsArray;
    mainObj["results"] = resultsArray;

    QJsonDocument doc(mainObj);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        // qWarning() << "Couldn't save file";
        throw std::runtime_error("Couldn't save file");
    }

    file.write(doc.toJson(QJsonDocument::Indented));
}

void Processor::load(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        // qWarning() << "Couldn't open file";
        throw std::runtime_error("Couldn't open file");
    }

    QByteArray saveData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(saveData);

    QJsonObject mainObj = doc.object();

    size_t rodIdPool = 1;
    QJsonArray rodArray = mainObj["rods"].toArray();
    for (auto it = rodArray.begin(); it != rodArray.end(); ++it) {
        Rod rod;
        rod.load(it->toObject());
        rod.setId(rodIdPool++);
        rods_.append(rod);
    }

    size_t sectionalIdPool = 1;
    QJsonArray sectionalForcesArray = mainObj["sectionalForces"].toArray();
    for (auto it = sectionalForcesArray.begin(); it != sectionalForcesArray.end(); ++it) {
        SectionalForce force;
        force.load(it->toObject());
        force.setId(sectionalIdPool++);
        sectionalForces_[force.rodId()] = force;
    }

    size_t longitudinalIdPool = 1;
    QJsonArray longitudinalForcesArray = mainObj["longitudinalForces"].toArray();
    for (auto it = longitudinalForcesArray.begin(); it != longitudinalForcesArray.end(); ++it) {
        LongitudinalForce force;
        force.load(it->toObject());
        force.setId(longitudinalIdPool++);
        longitudinalForces_.append(force);
    }

    QJsonArray pillarsArray = mainObj["pillars"].toArray();
    for (auto it = pillarsArray.begin(); it != pillarsArray.end(); ++it) {
        pillars_.append(it->toBool());
    }

    QJsonArray resultsArray = mainObj["results"].toArray();
    for (auto it = resultsArray.begin(); it != resultsArray.end(); ++it) {
        result_.append(it->toDouble());
    }
}

void Processor::bind(ConstructionGraphicsScene *scene) const
{
    for (auto &rod : rods_) {
        scene->appendRod(rod.length(), rod.sectionalArea());
    }

    for (auto &force : longitudinalForces_) {
        scene->appendLongitudinalForce(force.id(), force.nodeId(), force.force());
    }

    for (auto &force : sectionalForces_) {
        scene->appendSectionalForce(force.id(), force.rodId(), force.force());
    }

    bool firstPillar = pillars_.first();
    bool lastPillar = pillars_.last();

    if (firstPillar && lastPillar) {
        scene->setPillarSetup(ConstructionGraphicsScene::LeftAndRight);
    } else if (firstPillar) {
        scene->setPillarSetup(ConstructionGraphicsScene::Left);
    } else {
        scene->setPillarSetup(ConstructionGraphicsScene::Right);
    }
}

QVector<CalcType> Processor::result() const
{
    return result_;
}

int Processor::parts() const
{
    return rods_.size();
}

double Processor::partLength(int part) const
{
    return rods_[part].length();
}

double Processor::maxAllowableStrain(int part) const
{
    return rods_[part].allowableStrain();
}

CalcType Processor::movement(int part, CalcType x) const
{
    CalcType temp = (1 - x / rods_[part].length()) * result_[part]
            + x / rods_[part].length() * result_[part + 1];

    auto it = sectionalForces_.find(part);
    if (it != sectionalForces_.cend()) {
        temp += (it.value().force() * qPow(rods_[part].length(), 2))
                / (2 * rods_[part].elasticUnit() * rods_[part].sectionalArea())
                * (x / rods_[part].length()) * (1 - x / rods_[part].length());
    }

    return temp;
}

CalcType Processor::longitudinalForce(int part, CalcType x) const
{
    CalcType temp = rods_[part].elasticUnit() * rods_[part].sectionalArea() / rods_[part].length()
            * (result_[part + 1] - result_[part]);

    auto it = sectionalForces_.find(part);
    if (it != sectionalForces_.cend()) {
        temp += it.value().force() * rods_[part].length() / 2 * (1 - 2 * x / rods_[part].length());
    }

    return temp;
}

CalcType Processor::normalStrain(int part, CalcType x) const
{
    return longitudinalForce(part, x) / rods_[part].sectionalArea();
}
