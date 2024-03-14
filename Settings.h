//
// Created by Piotr Pszczółkowski on 14/03/2024.
//
#pragma once
#include <QSettings>
#include <QVariant>
#include <QString>
#include <optional>

class Settings : public QSettings {
public:
    Settings() : QSettings() {};
    ~Settings() override = default;
    Settings(Settings const&) = delete;
    Settings& operator=(Settings const&) = delete;

    bool save(QString const& key, QVariant const&& data) noexcept {
        setValue(key, data);
        return NoError == status();
    }

    std::optional<QVariant> read(QString const& key) noexcept {
        if (contains(key))
            if (auto data = value(key); status() == NoError)
                return std::move(data);
        return {};
    }
};
