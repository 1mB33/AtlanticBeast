#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <string_view>
#include <unordered_map>

// --------------------------------------------------------------------------------------------------------------------
enum EAssetType
{
    Unkonwn,
    Mesh,
    Texture,
    Sound,
};

// --------------------------------------------------------------------------------------------------------------------
struct AssetMetadata
{
    using Clock = ::std::chrono::system_clock;

    EAssetType        Type;
    Clock::time_point CreationTime;
    Clock::time_point LastModificationTime;
};

// --------------------------------------------------------------------------------------------------------------------
class Asset
{
    using Clock = ::std::chrono::system_clock;

  public:
    explicit Asset( AssetMetadata metadata = { .Type                 = EAssetType::Unkonwn,
                                               .CreationTime         = Clock::now(),
                                               .LastModificationTime = Clock::now() } )
      : m_Metadata( metadata )
      , m_pData( nullptr )
    {
    }

    ~Asset() = default;

  private:
    const AssetMetadata m_Metadata = {};
    void               *m_pData    = nullptr;
};

// --------------------------------------------------------------------------------------------------------------------
class ExampleAsset
{
  public:
    static ::std::string_view GetAssetsName()
    {
        return "ExampleAsset";
    }

    void DoAthing() const
    {
        ::std::cout << State << ::std::endl;
    }

    long long State = 0;
};

// --------------------------------------------------------------------------------------------------------------------
class AssetsReader
{
  public:
};

// --------------------------------------------------------------------------------------------------------------------
class AssetsManager
{
  public:
    AssetsManager() = default;

    ~AssetsManager()
    {
        ::std::cout << "List of loaded assets:" << ::std::endl;
        for ( auto &vk : m_vAssets )
            ::std::cout << vk.first << " " << vk.second->GetAssetsName() << ::std::endl;
    }

  public:
    ::std::weak_ptr<ExampleAsset> GenerateAsset()
    {
        ::std::cout << "Generating asset" << ::std::endl;

        const auto assetName = ExampleAsset::GetAssetsName();

        if ( m_vAssets.find( assetName ) != m_vAssets.end() )
        {
            return m_vAssets[ assetName ];
        }

        auto resultVal         = ::std::make_shared<ExampleAsset>();
        m_vAssets[ assetName ] = resultVal;
        return resultVal;
    }

  private:
    ::std::unordered_map<::std::string_view, ::std::shared_ptr<ExampleAsset>> m_vAssets = {};
};

// --------------------------------------------------------------------------------------------------------------------
using namespace std;

int main()
{
    cout << "START___" << endl;
    try
    {
        AssetsManager assetsManager = {};

        auto anAsset      = assetsManager.GenerateAsset().lock();
        auto anotherAsset = assetsManager.GenerateAsset().lock();

        anAsset->DoAthing();
        anotherAsset->DoAthing();

        assert( anAsset.get() == anotherAsset.get() );

        anAsset->State = chrono::system_clock::now().time_since_epoch().count();
    }
    catch ( ... )
    {
    }
    cout << "___END" << endl;
}
