#pragma once

#include <reference.hpp>

namespace Kyrys {
    class Item {
        typedef Kyrys::Enums::Item::MethodType MethodType;
        typedef Kyrys::Enums::Resolver::Mode Mode;
    private:
        MethodType m_methodType;
        QString m_name;
        QString m_nick;
        QString m_nickOriginal;
        QString m_passwordHash;
        int m_extension;
        int m_ID;

    public:
        /**
         * Default constructor, its purpose is mainly to reset item values.
         */
        Item();

        /**
         * @brief      Constructor of item, construct abstract item which holds info
         * about what should execute etc.
         *
         * @param[in]  json  The json is input json object containing data to populate item with.
         */
        Item(const QJsonObject &json);

        /**
         * @brief      Serializes item content to string. Good for saving to file.
         *
         * @param[in]  ID    ID of new entry in DB.
         *
         * @return     Returns serialized string.
         */
        std::string serialize(int ID) const;

        /**
         * @brief      Checks whether an item is valid, ie. contains invalid method name, empty nick or name
         * or any other invalid data. Can vary depending on which method is to be used.
         *
         * @return     True if valid, False otherwise.
         */
        int isValid() const;

        /**
         * If nick of the user has already been found in the DB,
         * add numbers to its end so it it unique.
         */
        void increaseNick();

        //Getters
        const MethodType &getMethodType() const;

        const QString &getName() const;

        const QString &getNick() const;

        const QString &getPasswordHash() const;

        int getID() const;

        //Setters
        void setID(int ID);
    };
}
