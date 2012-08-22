#ifndef BIRD_H
#define BIRD_H

#include <QString>

class Bird
{
public:
    Bird();

    Bird(int id,  const QString &finGroup, const QString &sweGroup, const QString &latinGroup,
          const QString &finName, const QString &sweName, const QString &latin, const QString &abbr,
          const QString &category);

    void setId(int id);
    void setFinGroup(const QString &group);
    void setSweGroup(const QString &group);
    void setEngGroup(const QString &group);
    void setLatinGroup(const QString &group);
    void setFinName(const QString &name);
    void setSweName(const QString &name);
    void setEngName(const QString &name);
    void setLatinName(const QString &name);
    void setAbbreviation(const QString &abbr);
    void setCategory(const QString &category);
    void setCustom(bool custom);

    int id() const;
    QString finGroup() const;
    QString sweGroup(bool allowEmpty = false) const;
    QString engGroup(bool allowEmpty = false) const;
    QString latinGroup() const;
    QString localizedName() const;
    QString localizedGroup() const;
    QString finName() const;
    QString sweName(bool allowEmpty = false) const;
    QString engName(bool allowEmpty = false) const;
    QString latinName() const;
    QString abbreviation() const;
    QString category() const;
    bool custom() const;

private:
    int mId;
    QString mFinGroup;
    QString mSweGroup;
    QString mEngGroup;
    QString mLatinGroup;
    QString mFinName;
    QString mSweName;
    QString mEngName;
    QString mLatinName;
    QString mAbbreviation;
    QString mCategory;
    bool mCustom;
};

#endif // BIRD_H
